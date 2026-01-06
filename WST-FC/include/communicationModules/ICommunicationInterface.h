#ifndef ICOMMUNICATIONINTERFACE
#define ICOMMUNICATIONINTERFACE
class ICommunicationInterface{
public:
    virtual ~ICommunicationInterface(){}
    virtual void Init();
    virtual void Loop();
};
#endif 