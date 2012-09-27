//
//  ViewController.h
//  gaode
//
//  Created by xuanyuan xidong on 12-5-8.
//  Copyright (c) 2012年 Carsmart. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MTMapView.h"
#import <CoreLocation/CoreLocation.h>
#import "MSearch.h"


@interface ViewController : UIViewController<MTMapView_Delegate,CLLocationManagerDelegate,MSearchDelegate,GPSToOffsetByPoint_Delegate>{
    
    //采集的数据,主要是从google地图中得到路线的坐标
    NSArray *routes;
    //线路使用的颜色
    UIColor *line_color;
    //路线,绘制为一张图片加载在这个view上
    UIImageView* route_view;
    
    MTMapView* map ;
    
    CLLocationManager *locManager;
    
//    MLONLAT poiXY;
    
    MSearch *search;

}

@property (nonatomic, retain) CLLocationManager *locManager;



@end
