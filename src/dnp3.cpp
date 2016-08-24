
#include "dnp3.h"

#include <asiodnp3/DNP3Manager.h>

dnp3::~dnp3()
{

}

using namespace std;
using namespace opendnp3;
using namespace openpal;
using namespace asiopal;
using namespace asiodnp3;


map<string, int> dnp3::objects;
mutex dnp3::objects_lock;

void dnp3::init_objects(void)
{
    lock_guard<mutex> lk(objects_lock);

    if (objects.empty())
    {
    	for(uint16_t i = 0; i< DNP3DataList.size();i++)
    	{
    		objects[DNP3DataList[i]->Name] = DNP3DataList[i]->DNP3Index;
    	}
    }
}
std::shared_ptr<IChannel> dnp3::create_dnp3_channel(std::string name, int port)
{
    static shared_ptr<DNP3Manager> manager = nullptr;
    static shared_ptr<IChannel> channel = nullptr;
    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);

    if (!manager)
    {
        const uint32_t FILTERS = levels::NORMAL | levels::ALL_COMMS;
        manager = std::make_shared<asiodnp3::DNP3Manager>(50, ConsoleLogger::Create());

        IChannel *ch = manager->AddTCPServer(name.c_str(), FILTERS, opendnp3::ChannelRetry::Default(), "0.0.0.0", port);
        channel.reset(ch);

        channel->AddStateListener([](ChannelState state) {
            std::cout << "channel state: " << ChannelStateToString(state) << std::endl;
        });
    }

    return channel;
}
void dnp3::Initialize(void)
{
    init_objects();

    channel = create_dnp3_channel(DNP3_OUTSTATION_Name, DNP3_Port);

    stackConfig = std::make_shared<OutstationStackConfig>();

    stackConfig->dbTemplate = DatabaseTemplate::AnalogOnly(22);
    stackConfig->outstation.eventBufferConfig = EventBufferConfig::AllTypes(22);

    stackConfig->outstation.params.allowUnsolicited = true;

    stackConfig->link.LocalAddr = LocalAddr;
    stackConfig->link.RemoteAddr = RemoteAddr;

    auto sc = stackConfig.get();
    outstation = channel->AddOutstation("outstation", SuccessCommandHandler::Instance(), DefaultOutstationApplication::Instance(), *sc);

    ConfigureDatabase(outstation->GetConfigView());

    outstation->Enable();
}

void dnp3::ConfigureDatabase(DatabaseConfigView view)
{
    view.analogs[0].variation = StaticAnalogVariation::Group30Var5;
    view.analogs[0].metadata.clazz = PointClass::Class2;
    view.analogs[0].metadata.variation = EventAnalogVariation::Group32Var7;
}


int dnp3::set_value(std::string name, double value) throw()
{
    int index;

    try
    {
        index = objects.at(name);
        MeasUpdate tx(outstation, UTCTimeSource::Instance().Now());
        tx.Update(Analog(value), index);
    }
    catch (std::out_of_range &e)
    {
        std::string msg = name + ": " + e.what();
        throw std::out_of_range(msg);
    }

    return 0;
}



std::list<std::string> dnp3::get_objects(void)
{
    std::list<std::string> result;

    init_objects();

    for (auto it = objects.begin(); it != objects.end(); it++)
        result.push_back(it->first);

    return result;
}


