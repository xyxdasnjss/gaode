//
//  ViewController.m
//  gaode
//
//  Created by xuanyuan xidong on 12-5-8.
//  Copyright (c) 2012年 Carsmart. All rights reserved.
//

#import "ViewController.h"
#import "MTPolylineOverlayView.h"
#import "MTAnnotationView.h"



@interface ViewController ()

@end

@implementation ViewController

@synthesize poiXY;
@synthesize options;


@synthesize locManager;

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
//    map = [MTMapView mapViewWithFrame:CGRectMake(0, 0, 320, 420) Delegate:0];

    map = [MTMapView mapViewWithFrame:self.view.frame Delegate:0];
    
    
    self.locManager = [[CLLocationManager alloc] init] ;
	self.locManager.desiredAccuracy = kCLLocationAccuracyBest;
    self.locManager.distanceFilter = 5.0f; // in meters
    self.locManager.delegate = self;
    [self.locManager startUpdatingLocation];
    
    
    search = [MSearch MSearchWithKey:@"c2b0f58a6f09cafd1503c06ef08ac7aeb7ddb91acc08d0bdfe899b9430866ff7fce8c67969e38170" delegate:self];
    
    
//    CGPoint mapcenter = CGPointMake(116.39716f, 39.91669f);
//    CGSize mapsize = CGSizeMake(0.04f, 0.03f);
//    CGRect region = CGRectMake(mapcenter.x-mapsize.width/2, mapcenter.y+mapsize.height/2, mapsize.width, mapsize.height);
//    
//    [map setRegion:region animated:NO];
//    
//    [map setCenterCoordinate:CGPointMake(116.39716f, 39.91669f+0.005) animated:YES];
//    map.showsUserLocation = YES;
    
//    map.userLocation.contentMode
    self.view = map;
    
//    MTPolylineOverlayView* polylineview = [MTPolylineOverlayView polylineView];
//    [polylineview addPointWithLongitude:region.origin.x Latitude:region.origin.y];
//    [polylineview addPointWithLongitude:region.origin.x+region.size.width Latitude:region.origin.y-region.size.height];
//    
//    [map addOverlay:polylineview];
//    
    
    
    
    CLLocationCoordinate2D coord1 = CLLocationCoordinate2DMake(39.92303400876581,116.33440017700195);
    CLLocationCoordinate2D coord2 = CLLocationCoordinate2DMake(39.95449073417355,116.27603530883789);
    
    
    UIImage* my = [UIImage imageNamed:@"pinRed"];
    CGRect imgFrame = CGRectMake(0, 0, my.size.width, my.size.height);
    MTAnnotationView* an = [MTAnnotationView annotationViewWithFrame:imgFrame UIImage:my Delegate:0];
    
    
    [an setUserInteractionEnabled:YES];
    [an setCoordinate:CGPointMake(116.33440017700195, 39.92303400876581)];
    [an setHotXY:CGPointMake(7, 34.5)];

    [map addAnnotation:an];
    
    
    MTAnnotationView* an1 = [MTAnnotationView annotationViewWithFrame:imgFrame UIImage:my Delegate:0];
    [an1 setCoordinate:CGPointMake(116.27603530883789, 39.95449073417355)];
    [an1 setHotXY:CGPointMake(7, 34.5)];
    
    [map addAnnotation:an1];
    
//    39.93244621864724,116.31036758422852|39.94751622278565,116.24736785888672&
    
    MTAnnotationView* an2 = [MTAnnotationView annotationViewWithFrame:imgFrame UIImage:my Delegate:0];
    [an2 setCoordinate:CGPointMake(116.31036758422852, 39.93244621864724)];
    [an2 setHotXY:CGPointMake(7, 34.5)];
    
    [map addAnnotation:an2];
    
    MTAnnotationView* an3 = [MTAnnotationView annotationViewWithFrame:imgFrame UIImage:my Delegate:0];
    [an3 setCoordinate:CGPointMake(39.93244621864724, 39.94751622278565)];
    [an3 setHotXY:CGPointMake(7, 34.5)];
    
    [map addAnnotation:an3];
    
    

    
    line_color = [UIColor redColor];
    
    route_view = [[UIImageView alloc]initWithFrame:map.frame];
    
    routes = [self calculate_routes_form:coord1 to:coord2];
    
    
    
    [self loadRoute];


    
    

}

-(void)locationManager:(CLLocationManager *)manager didUpdateToLocation:(CLLocation *)newLocation fromLocation:(CLLocation *)oldLocation{
    
    
    NSLog(@"++++%f,%f+++++",newLocation.coordinate.latitude,newLocation.coordinate.longitude);

//    [NSString stringWithFormat:@"%@f",newLocation.coordinate.latitude]
    
   
    CGPoint mapcenter = CGPointMake([[NSString stringWithFormat:@"%f",newLocation.coordinate.longitude] floatValue], [[NSString stringWithFormat:@"%f",newLocation.coordinate.latitude] floatValue]);
    CGSize mapsize = CGSizeMake(0.04f, 0.03f);
    CGRect region = CGRectMake(mapcenter.x-mapsize.width/2, mapcenter.y+mapsize.height/2, mapsize.width, mapsize.height);
    
    [map setRegion:region animated:NO];
    [map setShowsUserLocation:YES];
    [[map userLocation] startLocatingWithCycleTime:5.0f];   
	[[map userLocation] setCenterImage:[UIImage imageNamed:@"selfPnt.png"]];
    
//    http://search1.mapabc.com/sisserver?config=RGC&resType=xml&x1=116.400452&y1=39.943859&cr=0&flag=true&a_k=88fd41604cc3fa0cd81188420064db826765b9262c69b079923ae528b31c5296d39b4c26ce4637b2
    
    
    
    
    [map setCenterCoordinate:CGPointMake([[NSString stringWithFormat:@"%f",newLocation.coordinate.longitude] floatValue], [[NSString stringWithFormat:@"%f",newLocation.coordinate.latitude] floatValue]) animated:YES];
    
    UIImage* my = [UIImage imageNamed:@"pinRed"];
    CGRect imgFrame = CGRectMake(0, 0, my.size.width, my.size.height);
    MTAnnotationView* an = [MTAnnotationView annotationViewWithFrame:imgFrame UIImage:my Delegate:0];
    
    
    [an setUserInteractionEnabled:YES];
    [an setCoordinate:CGPointMake(newLocation.coordinate.longitude , newLocation.coordinate.latitude)];
    [an setHotXY:CGPointMake(7, 34.5)];
    
    [map addAnnotation:an];
    
//    MSearch* search = [MSearch MSearchWithKey:@"c2b0f58a6f09cafd1503c06ef08ac7aeb7ddb91a0fd4253a6f29c595c1ae10293bfa8b2abf6ce2fa" delegate:self];
    
//    而在获取到WGS84坐标后，就可以查询火星坐标了：
    poiXY.X = newLocation.coordinate.longitude; // 经度
    
//    NSLog(@"==%f==",poiXY.X);
    
    poiXY.Y = newLocation.coordinate.latitude; // 纬度
    [search GPSToOffSetByPoint:self];
//    这里不需要设置options属性，使用初始值即可。GPSToOffSetByPoint:方法会返回SEARCH_THREAD_ID，由于不需要停止线程，因此无视。
    

   
    
    
    
    
//	
//    self.view = map;
    
//    map.showsUserLocation = YES;
    
}
//    当获取到火星坐标后，就会调用GPSToOffsetResponse:方法了。这个方法的lonlat参数就是火星坐标了：
-(void)GPSToOffsetResponse:(MLONLAT)lonlat{
    
    
    NSLog(@"\n\n\n=====%f,%f====",lonlat.X,lonlat.Y);
    
    
    
}


-(void) loadRoute
{
        
    NSArray* pointStrings = routes;
    
    NSLog(@"111%@",pointStrings);
    
   MTPolylineOverlayView* polylineview = [MTPolylineOverlayView polylineView];
    
    for(int idx = 0; idx < pointStrings.count; idx++)
    {
        // break the string down even further to latitude and longitude fields.
        NSString* currentPointString = [pointStrings objectAtIndex:idx];
        NSArray* latLonArr = [currentPointString componentsSeparatedByCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@","]];
        
        CLLocationDegrees latitude = [[latLonArr objectAtIndex:0] doubleValue];
        CLLocationDegrees longitude = [[latLonArr objectAtIndex:1] doubleValue];
        
        // create our coordinate and add it to the correct spot in the array
       // CLLocationCoordinate2D coordinate = CLLocationCoordinate2DMake(latitude, longitude);
        
        
        [polylineview addPointWithLongitude:longitude Latitude:latitude];
                
        [map addOverlay:polylineview];
        

        
        
                
    }
    
        
    
    
    
    
} 



-(NSArray *)calculate_routes_form:(CLLocationCoordinate2D)f to:(CLLocationCoordinate2D)t{
    
    NSString* saddr = [NSString stringWithFormat:@"%f,%f",f.latitude,f.longitude];
    NSString* daddr = [NSString stringWithFormat:@"%f,%f",t.latitude,t.longitude];
    
    
    
    
    
    NSString* apiUrlStr = [NSString stringWithFormat:@"http://maps.google.com/maps/api/directions/json?origin=%@&destination=%@&waypoints=39.93244621864724,116.31036758422852|39.94751622278565,116.24736785888672&sensor=true&mode=driving",saddr,daddr];
    
    apiUrlStr = [apiUrlStr stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    
    //    NSLog(@"%@",apiUrlStr);
    
    NSURL* apiUrl = [NSURL URLWithString:apiUrlStr];
    

    NSLog(@"url==%@",apiUrl);
 

	NSMutableURLRequest *urlRequest = [NSMutableURLRequest requestWithURL:apiUrl];
   
	NSError *error;
	NSURLResponse *response;
	NSData *tw_result = [NSURLConnection sendSynchronousRequest:urlRequest returningResponse:&response error:&error];
	
    NSLog(@"%@",tw_result);
    
    NSString *result = nil;
    if(error){
//        NSLog(@"something is wrong: %@", [error description]);
    }else{
        if(tw_result!=nil){
            
            
            //gbk
//            NSStringEncoding enc = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGB_18030_2000);
//            result = [[NSString alloc] initWithData:tw_result encoding:enc];
            //utf-8
            result = [[NSString alloc] initWithData:tw_result encoding:NSUTF8StringEncoding];
            NSLog(@"%@",tw_result);
            
            NSDictionary *dic_data = [NSJSONSerialization JSONObjectWithData:[result dataUsingEncoding:NSUnicodeStringEncoding] options:NSJSONReadingMutableLeaves error:&error];
            
            
            
            NSArray *dic_routes = [dic_data objectForKey:@"routes"];
            
            NSDictionary *legs = [dic_routes objectAtIndex:0];
            
            NSDictionary *temp = [legs objectForKey:@"overview_polyline"];
            NSString *end_points = [temp objectForKey:@"points"];
            
            NSLog(@"===:%@",end_points);
            
             return [self decodePolyLine:[end_points mutableCopy]];
        }
    }

    return nil;
    
    
//    ASIHTTPRequest *request = [ASIHTTPRequest requestWithURL:apiUrl];
//    [request startSynchronous];
//    NSString *apiResponse = [request responseString];
//    
//    NSLog(@"%@",apiResponse);
//    
//    NSError *error;
  
    
   
    
    
}

-(NSMutableArray *)decodePolyLine:(NSMutableString *)encoded{
    NSInteger len = [encoded length];
    NSInteger index = 0;
    NSMutableArray *array = [[NSMutableArray alloc]init];
    NSInteger lat = 0 ;
    NSInteger lng = 0;
    while (index<len) {
        NSInteger b;
        NSInteger shift = 0;
        NSInteger result = 0;
        do {
            b =[ encoded characterAtIndex:index++]-63;
            result |= (b&0x1f)<<shift;
            shift+=5;
            
        } while (b>=0x20);
        NSInteger dlat = ((result&1)?~(result>>1):(result>>1));
        lat +=dlat;
        shift = 0;
        result = 0;
        do {
            b =[ encoded characterAtIndex:index++]-63;
            result |= (b&0x1f)<<shift;
            shift+=5;
            
        } while (b>=0x20);
        NSInteger dlng = ((result &1)?~(result>>1):(result>>1));
        lng +=dlng;
        NSNumber *latitude = [[NSNumber alloc]initWithFloat:lat *1e-5];
        NSNumber *longitude = [[NSNumber alloc]initWithFloat:lng *1e-5];
        
        NSLog(@"%@",latitude);
        NSLog(@"%@",longitude);
        
 
        NSString *str = [NSString stringWithFormat:@"%@,%@",latitude,longitude];
        [array addObject:str];
        
    }
    return array;
    
}





- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

-(NSString*)userKeyForMTMapView{
    
    return @"c2b0f58a6f09cafd1503c06ef08ac7aeb7ddb91a63e8b04221ce3df0c3372810c04826dc3abfb753";
}

-(void)mapViewAuthResult:(BOOL)result{
    
    
}

//- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
//{
//    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
//}

@end
