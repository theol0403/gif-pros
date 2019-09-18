#include "main.h"
#include "gif-pros/gifclass.hpp"

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

	lv_obj_t * obj1;
	obj1 = lv_obj_create(lv_scr_act(), NULL);
	lv_obj_set_size(obj1, 200, 200);
	lv_obj_set_style(obj1, &lv_style_plain_color);
	lv_obj_align(obj1, NULL, LV_ALIGN_CENTER, 0, 0);

	Gif gif("/usd/earth.gif", obj1, Gif::Transparency::off);

	while (true) {
		pros::delay(20);
	}
}
