#include "g96.h"
#include <math.h>

G96_Status g96_calculate_speed(const G96_Context *ctx, double *out_rpm) {
    // 1. G97 模式处理：直接应用倍率输出设定值
    if (!ctx->g96_enabled) {
        *out_rpm = ctx->s_set * ctx->spindle_ovr;
        // 应用主轴速度范围限制
        if (*out_rpm > ctx->spindle_max_speed) {
            *out_rpm = ctx->spindle_max_speed;
            return G96_STATUS_SPEED_EXCEED;
        }
        if (*out_rpm < ctx->spindle_min_speed) {
            *out_rpm = ctx->spindle_min_speed;
        }
        return G96_STATUS_OK;
    }

    // 2. 检查 S 值是否有效
    if (ctx->s_set <= 0) return G96_STATUS_ERR_S_ZERO;

    // 3. 计算实际半径并应用保护
    double radius = fabs(ctx->pos_fb);
    if (ctx->is_diameter == 1) radius /= 2.0; 
    if (radius < ctx->min_radius) radius = ctx->min_radius;

    // 4. 计算恒线速转速: n = 159.155 * S / R
    double raw_rpm = 159.1549431 * ctx->s_set / radius;

    // 5. 确定转速上限 (取机械上限、G50 和主轴最大速度的交集)
    double upper_limit = ctx->max_rpm_limit;
    if (ctx->g50_limit < upper_limit) upper_limit = ctx->g50_limit;
    if (ctx->spindle_max_speed < upper_limit) upper_limit = ctx->spindle_max_speed;

    // 6. 执行钳位逻辑
    G96_Status status = G96_STATUS_OK;
    if (ctx->clamp_mode == 0) { // 模式 0: 倍率前钳位
        double clamped = (raw_rpm > upper_limit) ? upper_limit : raw_rpm;
        clamped = (clamped < ctx->spindle_min_speed) ? ctx->spindle_min_speed : clamped;
        *out_rpm = clamped * ctx->spindle_ovr;
    } else { // 模式 1: 倍率后钳位
        double with_ovr = raw_rpm * ctx->spindle_ovr;
        if (with_ovr > upper_limit) {
            *out_rpm = upper_limit;
            status = G96_STATUS_SPEED_EXCEED;
        } else if (with_ovr < ctx->spindle_min_speed) {
            *out_rpm = ctx->spindle_min_speed;
        } else {
            *out_rpm = with_ovr;
        }
    }
    
    // 如果是旋转轴(1-5)，可根据轴类型进行特殊处理
    if (ctx->axis_type >= 1 && ctx->axis_type <= 5) {
        // 对于旋转轴可在此处添加特定的处理逻辑
        // 目前暂无特殊处理，保留扩展接口
    }
    
    return status;
}