#import <Foundation/Foundation.h>

//base class I need to derive from and add multiple step initialization
@interface Base : NSObject
/*
 base class declaration
 */
+(instancetype)createInstance:(NSString *)localPath data:(NSDictionary *)data;
-(void)addToPool;
-(void)print;
-(instancetype)initWithData:(NSDictionary *)data;
@end

@implementation Base{
    @protected
    __strong NSDictionary * _data;
    __strong NSString * _localPath;
}

+(instancetype)createInstance:(NSString *)localPath data:(NSDictionary *)data{
    id retVal=[[self alloc] initWithData:data];
    ((Base *)retVal)->_localPath=localPath;
    [retVal addToPool];
    return retVal;
}
-(instancetype)initWithData:(NSDictionary *)data{return [super init];}
-(void)addToPool{}
-(void)print{NSLog(@"Base");}
@end

//derived level1
@interface Derived1 : Base
@end
@implementation Derived1
-(instancetype)initWithData:(NSDictionary *)data{
    self=[super init];
    if (self)
        _data=data;
    return self;
}
-(void)print{printf("Derived1\n");}
@end

//derived level 2
@interface Derived2 : Derived1
@end
@implementation Derived2
-(instancetype)initWithData:(NSDictionary *)data{return [super initWithData:data];}
-(void)print{printf("Derived2\n");}
@end

//derived level 3
@interface Derived3 : Derived2
@end
@implementation Derived3
-(instancetype)initWithData:(NSDictionary *)data{return [super initWithData:data];}
-(void)print{printf("Derived3\n");}
@end


int main(int argc, const char * argv[]){
    @autoreleasepool {
        Derived1 * derived1=[Derived1 createInstance:@"/" data:nil];
        Derived2 * derived2=[Derived2 createInstance:@"/" data:nil];
        Derived3 * derived3=[Derived3 createInstance:@"/" data:nil];
        
        [derived1 print];
        [derived2 print];
        [derived3 print];
    }
    return 0;
}
/*
 Output:
 
 Derived1
 Derived2
 Derived3
 */