#ifndef CONFIG_H
#define CONFIG_H
#include <QString>


const QString DATA_PATH = "/data";
const QString DB_PATH = DATA_PATH + "/database";
const QString PHOTO_PATH = DATA_PATH + "/photos";
const QString DB_NAME = "/project.db";
const QString TEMP_PATH = DATA_PATH + "/temp";

//speech
const QString ADD_SUCCESS = "添加成功";
const QString ADD_FAIL = "添加失败";
const QString OUT_HOUSE_SUCCESS = "出库成功";
const QString OUT_HOUSE_FAIL = "出库失败";
const QString ALREADY_EXIST = "该快递已出库";
const QString BARCODE_EMPTY = "快递单号不能为空";
const QString SAVE_PHOTO_FAIL = "保存照片失败，请重新拍照";

#endif // CONFIG_H
