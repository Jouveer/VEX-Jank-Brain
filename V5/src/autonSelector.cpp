#include "main.h"

// 480 x 240 Screen
lv_obj_t *cbEnabled = lv_cb_create(lv_scr_act(), NULL);
lv_obj_t *cbSimple = lv_cb_create(lv_scr_act(), NULL);

lv_obj_t *labelEnabled = lv_label_create(lv_scr_act(), NULL);
lv_obj_t *labelSimple = lv_label_create(lv_scr_act(), NULL);

lv_obj_t *labelTitle = lv_label_create(lv_scr_act(), NULL);
lv_obj_t *labelColourSelection = lv_label_create(lv_scr_act(), NULL);
lv_obj_t *labelRobotSelection = lv_label_create(lv_scr_act(), NULL);

lv_obj_t *btnRedAuton = lv_btn_create(lv_scr_act(), NULL);
lv_obj_t *btnBlueAuton = lv_btn_create(lv_scr_act(), NULL);
lv_obj_t *labelRedButton = lv_label_create(btnRedAuton, NULL);
lv_obj_t *labelBlueButton = lv_label_create(btnBlueAuton, NULL);

lv_obj_t *btnSusAuton = lv_btn_create(lv_scr_act(), NULL);
lv_obj_t *btnGusAuton = lv_btn_create(lv_scr_act(), NULL);
lv_obj_t *labelSusButton = lv_label_create(btnSusAuton, NULL);
lv_obj_t *labelGusButton = lv_label_create(btnGusAuton, NULL);

lv_obj_t *labelGyro = lv_label_create(lv_scr_act(), NULL);

lv_coord_t topButtonOffset = 55;
lv_coord_t bottomButtonOffet = 130;

bool autonEnabled = true;
bool autonSimple = false;
bool autonRed = true;
bool autonGustavo = true;

static lv_res_t autonColour(lv_obj_t *btn) {
    uint8_t id = lv_obj_get_free_num(btn);
    if (id == 1) {
        lv_btn_set_state(btnRedAuton, LV_BTN_STATE_TGL_REL);
        lv_btn_set_state(btnBlueAuton, LV_BTN_STATE_REL);
        autonRed = true;
    } else if (id == 2) {
        lv_btn_set_state(btnBlueAuton, LV_BTN_STATE_TGL_REL);
        lv_btn_set_state(btnRedAuton, LV_BTN_STATE_REL);
        autonRed = false;
    }
    return LV_RES_OK;  // Return OK if the button is not deleted
}

static lv_res_t autonRobot(lv_obj_t *btn) {
    uint8_t id = lv_obj_get_free_num(btn);
    if (id == 3) {
        lv_btn_set_state(btnSusAuton, LV_BTN_STATE_TGL_REL);
        lv_btn_set_state(btnGusAuton, LV_BTN_STATE_REL);
        autonGustavo = true;
        arcadeDrive = false;
    } else if (id == 4) {
        lv_btn_set_state(btnSusAuton, LV_BTN_STATE_REL);
        lv_btn_set_state(btnGusAuton, LV_BTN_STATE_TGL_REL);
        autonGustavo = false;
        autonSimple = true;
        arcadeDrive = true;
        lv_cb_set_checked(cbSimple, true);
    }
    return LV_RES_OK;  // Return OK if the button is not deleted
}

static lv_res_t enableAuton(lv_obj_t *cb) {
    autonEnabled = lv_cb_is_checked(cb);
    return LV_RES_OK;
}

static lv_res_t enableSimple(lv_obj_t *cb) {
    autonSimple = lv_cb_is_checked(cb);
    return LV_RES_OK;
}

void drawAutonSelector() {
    lv_label_set_text(labelTitle, "Autonomous Selection");
    lv_obj_align(labelTitle, NULL, LV_ALIGN_CENTER, 0, -100);

    lv_label_set_text(labelColourSelection, "Colour Selection");
    lv_obj_align(labelColourSelection, NULL, LV_ALIGN_CENTER, 55, -65);

    lv_label_set_text(labelRobotSelection, "Robot Selection");
    lv_obj_align(labelRobotSelection, NULL, LV_ALIGN_CENTER, -142, -65);

    //--------------------------------------------------------------------------------------------------------------

    lv_label_set_text(labelRedButton, "RED");
    lv_obj_set_free_num(btnRedAuton, 1); /*Set a unique number for the button*/
    lv_btn_set_action(btnRedAuton, LV_BTN_ACTION_CLICK, autonColour);
    lv_obj_set_size(btnRedAuton, LV_HOR_RES / 4, LV_VER_RES / 6);
    lv_btn_set_state(btnRedAuton, LV_BTN_STATE_TGL_REL); /*Set toggled state*/
    lv_obj_align(btnRedAuton, labelColourSelection, LV_ALIGN_CENTER, 0, topButtonOffset);

    lv_label_set_text(labelBlueButton, "BLUE");
    lv_obj_set_free_num(btnBlueAuton, 2); /*Set a unique number for the button*/
    lv_btn_set_action(btnBlueAuton, LV_BTN_ACTION_CLICK, autonColour);
    lv_obj_set_size(btnBlueAuton, LV_HOR_RES / 4, LV_VER_RES / 6);
    lv_btn_set_state(btnBlueAuton, LV_BTN_STATE_REL); /*Set toggled state*/
    lv_obj_align(btnBlueAuton, labelColourSelection, LV_ALIGN_CENTER, 0, bottomButtonOffet);

    //--------------------------------------------------------------------------------------------------------------

    lv_label_set_text(labelSusButton, "GUSTAVO"); //Yes I know....
    lv_obj_set_free_num(btnSusAuton, 3); /*Set a unique number for the button*/
    lv_btn_set_action(btnSusAuton, LV_BTN_ACTION_CLICK, autonRobot);
    lv_obj_set_size(btnSusAuton, LV_HOR_RES / 4, LV_VER_RES / 6);
    lv_btn_set_state(btnSusAuton, LV_BTN_STATE_TGL_REL); /*Set toggled state*/
    lv_obj_align(btnSusAuton, labelRobotSelection, LV_ALIGN_CENTER, 0, topButtonOffset);

    lv_label_set_text(labelGusButton, "SUSPICIOUS"); //Yes I know....
    lv_obj_set_free_num(btnGusAuton, 4); /*Set a unique number for the button*/
    lv_btn_set_action(btnGusAuton, LV_BTN_ACTION_CLICK, autonRobot);
    lv_obj_set_size(btnGusAuton, LV_HOR_RES / 4, LV_VER_RES / 6);
    lv_btn_set_state(btnGusAuton, LV_BTN_STATE_REL); /*Set toggled state*/
    lv_obj_align(btnGusAuton, labelRobotSelection, LV_ALIGN_CENTER, 0, bottomButtonOffet);

    //--------------------------------------------------------------------------------------------------------------

    lv_label_set_text(labelEnabled, "Enabled");
    lv_obj_align(labelEnabled, NULL, LV_ALIGN_CENTER, 195, -50);

    lv_label_set_text(labelSimple, "Simple");
    lv_obj_align(labelSimple, NULL, LV_ALIGN_CENTER, 195, 50);

    lv_cb_set_text(cbEnabled, "");
    lv_cb_set_action(cbEnabled, enableAuton);
    lv_cb_set_checked(cbEnabled, true);
    lv_obj_align(cbEnabled, labelEnabled, LV_ALIGN_CENTER, 5, 30);

    lv_cb_set_text(cbSimple, "");
    lv_cb_set_action(cbSimple, enableSimple);
    lv_cb_set_checked(cbSimple, false);
    lv_obj_align(cbSimple, labelSimple, LV_ALIGN_CENTER, 5, 30);

    //--------------------------------------------------------------------------------------------------------------

    lv_label_set_text(labelGyro, "");
    lv_obj_align(labelGyro, btnGusAuton, LV_ALIGN_CENTER, -37, 40);

    static lv_point_t line_points[] = {{0, 0}, {480, 0}};
    lv_obj_t *line;
    line = lv_line_create(lv_scr_act(), NULL);
    lv_line_set_points(line, line_points, 2); /*Set the points*/
    lv_obj_align(line, NULL, LV_ALIGN_CENTER, 0, -85);

    static lv_point_t line_points2[] = {{0, 35}, {0, 240}};
    line = lv_line_create(lv_scr_act(), NULL);
    lv_line_set_points(line, line_points2, 2); /*Set the points*/
    lv_obj_align(line, NULL, LV_ALIGN_CENTER, 150, 0);

    static lv_point_t line_points3[] = {{0, 35}, {0, 240}};
    line = lv_line_create(lv_scr_act(), NULL);
    lv_line_set_points(line, line_points3, 2); /*Set the points*/
    lv_obj_align(line, NULL, LV_ALIGN_CENTER, -40, 0);
}