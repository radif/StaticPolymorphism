StaticPolymorphism
==================

c++ approach to the objective-c polymorphic multistep initialization using Curiously Recursive Template Pattern with multiple inheritance. This example shows how to rely solely on templates while completely avoiding the use of Macro or repetitious code.

Unlike c++, objective-c offers us an array of great mechanisms such as reflection and partial duck typing to be able to write multistep instantiation type of factory methods.
To achieve this goal, in objective-c you can do something like this in the base class:

	+(instancetype)createInstance:(NSString *)localPath data:(NSDictionary *)data{
	    id retVal=[[self alloc] initWithData:data];
	    ((Base *)retVal)->_localPath=localPath;
	    return retVal;
	}
	
While "self" refers to the correct class object, no matter which subclass you are addressing, "id" and "instancetype" provide us necessary flexibility and compile time security.

To instantiate such object all you have to do is the following:

	Derived * derived=[Derived createInstance:@"/" data:nil];
	
"self" of the base class will turn into the instance of "Derived".

c++ doesn't have a class object. In order to implement the same pattern in c++ it is possible to add a "createInstance" static method into each subclass or create a macro that does that for you, taking the type argument. Alternatively it is possible to leverage the Curiously Recursive Template Pattern and multiple inheritance to achieve the same goal. Additionally c++ comes more powerful with the ability to add pure virtual functions to enforce certain methods such as init();

The base class should be a template

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


Starting level2, derived classes must specify which createInstance function they use; see "using" qualifier:

	//derived level2
	class Derived2 : public Derived1, public PlatformSupport::Factory<Derived2>{
	    typedef Derived1 super;
	public:
	    using PlatformSupport::Factory<Derived2>::createInstance;//using qualifier is necessary to avoid ambiguity for different base classes
	    virtual bool init(void * data){ return super::init(data);}
	    void print(){std::cout << "Derived2"<<std::endl;};
	};

instantiation goes exactly as in objective-c and there is no need to add createInstance static function into each derived class:

	auto derived1(Derived1::createInstance("/", nullptr));
	auto derived2(Derived2::createInstance("/", nullptr));
	auto derived3(Derived3::createInstance("/", nullptr));

or

	Derived1 * derived1=Derived1::createInstance("/", nullptr);
	Derived2 * derived2=Derived2::createInstance("/", nullptr);
	Derived3 * derived3=Derived3::createInstance("/", nullptr);

obj-c example for comparison

	Derived1 * derived1=[Derived1 createInstance:@"/" data:nil];
	Derived2 * derived2=[Derived2 createInstance:@"/" data:nil];
	Derived3 * derived3=[Derived3 createInstance:@"/" data:nil];

