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
const QString IN_SUCCESS = "入库成功";
const QString OUT_SUCCESS = "出库成功";
const QString ALREADY_EXIST = "该快递已出库";
const QString BARCODE_EMPTY = "快递单号不能为空";
const QString SAVE_PHOTO_FAIL = "保存照片失败，请重新拍照";
const QString ITEM_EXIST = "快递已入库，无需重新入库";
const QString IN_ERROR = "入库失败";
const QString OUT_ERROR = "出库失败";
const QString NO_SUCH_ITEM = "仓库中无该快递";

//barcode
static const char *BARCODE_READER_PY_MODULE = "barcodeReader";
static const char *BARCODE_READER_PY_FUN = "readBarcode";

#endif // CONFIG_H
