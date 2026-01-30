#ifndef G96_H
#define G96_H

/**
 * G96 计算上下文：主轴组件需每周期填充此结构体
 */
typedef struct {
    double s_set;           // 线速度设定值 (m/min)
    double pos_fb;          // 选定基准轴的实时反馈位置 (X/Y/Z)
    double g50_limit;       // G50 设定的转速上限
    double spindle_ovr;     // 实时主轴倍率 (1.0 = 100%)
    
    // 新增主轴速度限制
    double spindle_max_speed;  // 主轴最大速度限制
    double spindle_min_speed;  // 主轴最小速度限制
    
    // 新增轴类型定义
    int axis_type;             // 轴类型 (0:线性轴, 1-5:旋转轴)
    
    // 内部参数（通过参数接口访问）
    double max_rpm_limit;   // 机械允许的物理最高转速
    double min_radius;      // 最小保护半径
    int clamp_mode;         // 钳位模式 (0:前钳位, 1:后钳位)
    int is_diameter;        // 坐标类型 (0:半径轴, 1:直径轴)
    int g96_enabled;        // 模式开关 (1:G96, 0:G97)
} G96_Context;
/**
 * 报警码定义
 */
typedef enum {
    G96_STATUS_OK = 0,
    G96_STATUS_SPEED_EXCEED = 1, // 超过最大限速报警
    G96_STATUS_ERR_S_ZERO = 2    // 未指定转速报警
} G96_Status;

/**
 * G96 核心算法函数接口
 */
G96_Status g96_calculate_speed(const G96_Context *ctx, double *out_rpm);

#endif