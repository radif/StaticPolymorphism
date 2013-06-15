#include <iostream>

//base class I need to derive from and add multiple step initialization
class Base {
    /*
     base class declaration
     */
public:
    void addToPool(){/*...*/}
protected:
    void * _data=nullptr;
};

namespace PlatformSupport{
    class Path{
    protected:
        std::string _localPath;
    };
    
    //Factory is a templetized abstract base class that will add "createInstance" method into each derived class and instantiate the correct type
    template<class C>
    class Factory : protected virtual PlatformSupport::Path{ // deriving from virtual Path to prevent multiple inclusions in subclasses
    public:
        static C * createInstance(const std::string & localPath, void * data){
            auto retVal(new C);
            if (retVal) {
                retVal->_localPath=localPath;
                if (retVal->init(data))
                    retVal->addToPool();
                else{
                    delete retVal;
                    retVal=nullptr;
                }
            }
            return retVal;
        }
        
    protected:
        virtual bool init(void * data)=0;
    };
}

//derived level1
class Derived1 : public Base, public PlatformSupport::Factory<Derived1>{
    typedef Base super;
public:
    //no need to use "using" qualifier
    virtual bool init(void * data){_data=data; return true;}
    void print(){std::cout << "Derived1"<<std::endl;};
};

//derived level2
class Derived2 : public Derived1, public PlatformSupport::Factory<Derived2>{
    typedef Derived1 super;
public:
    using PlatformSupport::Factory<Derived2>::createInstance;//using qualifier is necessary to avoid ambiguity for different base classes
    virtual bool init(void * data){ return super::init(data);}
    void print(){std::cout << "Derived2"<<std::endl;};
};

//derived level3
class Derived3 : public Derived2, public PlatformSupport::Factory<Derived3>{
    typedef Derived2 super;
public:
    using PlatformSupport::Factory<Derived3>::createInstance;//using qualifier is necessary to avoid ambiguity for different base classes
    virtual bool init(void * data){return super::init(data);}
    void print(){std::cout << "Derived3"<<std::endl;};
};

    



int main(int argc, const char * argv[]){
    auto derived1(Derived1::createInstance("/", nullptr));
    auto derived2(Derived2::createInstance("/", nullptr));
    auto derived3(Derived3::createInstance("/", nullptr));
    
    derived1->print();
    derived2->print();
    derived3->print();
    return 0;
}

/*
 Output:
 
 Derived1
 Derived2
 Derived3
 */
