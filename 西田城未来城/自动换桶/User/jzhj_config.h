#ifndef __ZJHJ_CONFIG_H__
#define __ZJHJ_CONFIG_H__

#define SYSTEM_CHANNEL_NUMBERS 		5

#define DEFAULT_TOTAL_DISTANCE 		1.692f 				// 自动换桶总的距离缺省值 		单位：米
#define DUSTBIN_NUMBERS 			3					// 自动换桶垃圾桶个数缺省值
#define EACH_LAP 					0.072f 				// 步进电机每圈行走距离 		单位：米

#define FULL_DETECT_DISTANCE 		0.35f				// 满桶检测距离垃圾投放距离 	单位：米

#define DUSTBIN1_DISTANCE 			0.2f				// 第一个垃圾桶投放位置 		单位：米
#define DUSTBIN2_DISTANCE 			1.0f//0.76f				// 第二个垃圾桶投放位置 		单位：米
#define DUSTBIN3_DISTANCE 			1.685f//1.525f				// 第三个垃圾桶投放位置 		单位：米
#define DUSTBIN4_DISTANCE 			(1.685f + 0.5f)

#endif
