#include <stdio.h>
#include "g96.h"

int main() {
    // 模拟一个典型的 G96 环境
    G96_Context my_ctx = {
        .s_set = 150.0, 
        .pos_fb = 50.0, 
        .g50_limit = 2500.0, 
        .spindle_ovr = 1.0,
        .max_rpm_limit = 5000.0, 
        .min_radius = 0.001, 
        .clamp_mode = 0, 
        .is_diameter = 1, 
        .g96_enabled = 1,
        // 新增变量的初始化
        .spindle_max_speed = 3000.0,  // 主轴最大转速限制
        .spindle_min_speed = 10.0,    // 主轴最小转速限制
        .axis_type = 0                // 0表示线性轴
    };

    double result_rpm;
    G96_Status s = g96_calculate_speed(&my_ctx, &result_rpm);
    
    printf("状态码: %d (0代表正常)\n", s);
    
    // 根据坐标类型(is_diameter)调整输出文本
    if(my_ctx.is_diameter == 1) {
        printf("坐标为 %.2fmm(直径) 时，计算输出转速: %.2f RPM\n", my_ctx.pos_fb, result_rpm);
    } else {
        printf("坐标为 %.2fmm(半径) 时，计算输出转速: %.2f RPM\n", my_ctx.pos_fb, result_rpm);
    }
    
    return 0;
}