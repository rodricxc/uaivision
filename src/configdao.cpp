#include "configdao.h"

ConfigDAO* ConfigDAO::object = NULL;

ConfigDAO::ConfigDAO(QObject *parent) : QObject(parent) {
  this->fileName = QString("../src/config/UaiVision.json");
  cout << "Starting dao with config file from: " << fileName.toStdString() << endl;
  this->readJSON();
}
int ConfigDAO::getFieldBorder() const
{
  return fieldBorder;
}

void ConfigDAO::setFieldBorder(int value)
{
  fieldBorder = value;
}

int ConfigDAO::getFieldHeight() const
{
  return fieldHeight;
}

void ConfigDAO::setFieldHeight(int value)
{
  fieldHeight = value;
}

int ConfigDAO::getFieldWidth() const
{
  return fieldWidth;
}

void ConfigDAO::setFieldWidth(int value)
{
  fieldWidth = value;
}

int ConfigDAO::getFieldGoalDepth() const
{
  return fieldGoalDepth;
}

void ConfigDAO::setFieldGoalDepth(int value)
{
  fieldGoalDepth = value;
}

int ConfigDAO::getFieldGoalSize() const
{
  return fieldGoalSize;
}

void ConfigDAO::setFieldGoalSize(int value)
{
  fieldGoalSize = value;
}


QPoint ConfigDAO::getCornerRightBottom() const {
  return cornerRightBottom;
}

void ConfigDAO::setCornerRightBottom(const QPoint &value) {
  cornerRightBottom = value;
}

QPoint ConfigDAO::getCornerRightTop() const {
  return cornerRightTop;
}

void ConfigDAO::setCornerRightTop(const QPoint &value) {
  cornerRightTop = value;
}

QPoint ConfigDAO::getCornerLeftBottom() const {
  return cornerLeftBottom;
}

void ConfigDAO::setCornerLeftBottom(const QPoint &value) {
  cornerLeftBottom = value;
}

QPoint ConfigDAO::getCornerLeftTop() const {
  return cornerLeftTop;
}

void ConfigDAO::setCornerLeftTop(const QPoint &value) {
  cornerLeftTop = value;
}


bool ConfigDAO::readJSON() {

  QString json = readFile(this->fileName);
  if (json.isEmpty()) {
      cout << "Could not read JSON file!"<< endl;
      return false;
    }

  QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
  if (d.isNull()) {
      cout << "Error on parsing JSON file! Check for errors in the config file." << endl;
      return false;
    }

  if (d.isArray()) {
      cout << "Error on parsing JSON file! You should have a file with a JSON Object, but we got a JSON Array of Objects." << endl;
      return false;
    }

  if (d.isObject()) {
      this->mainObjectJSON = d.object();
      cout << "Config Name: " <<  mainObjectJSON["name"].toString().toStdString() << endl;

      QJsonArray jsonArray = mainObjectJSON["colorSpaces"].toArray();

      this->colorSpaces.clear();
      foreach (const QJsonValue & value, jsonArray) {
          QJsonObject obj = value.toObject();
          ColorSpace c = jsonToColorSpace(obj);
          this->colorSpaces.push_back(c);
        }

      QJsonObject bord= mainObjectJSON["fieldBorders"].toObject();

      this->setCornerLeftTop(this->jsonToQPoint(bord["topLeft"].toObject()));
      this->setCornerRightTop(this->jsonToQPoint(bord["topRight"].toObject()));
      this->setCornerLeftBottom(this->jsonToQPoint(bord["bottomLeft"].toObject()));
      this->setCornerRightBottom(this->jsonToQPoint(bord["bottomRight"].toObject()));

      QJsonObject field= mainObjectJSON["field"].toObject();
      this->setFieldBorder(field["border"].toInt());
      this->setFieldWidth(field["width"].toInt());
      this->setFieldHeight(field["height"].toInt());
      this->setFieldGoalDepth(field["goalDepth"].toInt());
      this->setFieldGoalSize(field["goalSize"].toInt());

      //cout << "left top: " <<getCornerLeftTop().x() << ", " << getCornerLeftTop().y() << endl;

      //cout << "num of color spaces: " << colorSpaces.size() << endl;

    } else {
      cout << "ERROR!!!" << endl;
      return false;
    }

  return true;

}

bool ConfigDAO::writeJSON() {
  cout << "Writing '" <<  mainObjectJSON["name"].toString().toStdString() << "' to the file: " << this->fileName.toStdString() << endl;

  QFile f(this->fileName);
  QJsonDocument d;
  d.setObject(this->mainObjectJSON);
  f.open(QFile::WriteOnly);
  f.write(d.toJson());
  f.close();
  cout << "Done!" << endl;
}


ColorSpace ConfigDAO::insertColorSpace(ColorSpace cs) {

  this->colorSpaces.push_back(cs);
  this->lastModified = cs;
  return cs;
}

vector<ColorSpace> ConfigDAO::getColorSpaces() const
{
  return colorSpaces;
}


bool ConfigDAO::hasColor(QString colorName) {
  vector<ColorSpace>::iterator it;
  ColorSpace i;
  for (it = this->colorSpaces.begin(); it != this->colorSpaces.end(); it++){
      if (it->getName().compare(colorName) == 0) {
          return true;
        }
    }
  return false;
}


ConfigDAO *ConfigDAO::Instance() {
  if (!object) {
      object = new ConfigDAO();
    }

  return object;

}

QString ConfigDAO::readFile(const QString &filename) {
  QFile f(filename);
  if (!f.open(QFile::ReadOnly | QFile::Text)) {
      return QString();
    } else {
      QTextStream in(&f);
      return in.readAll();
    }
}


ColorSpace ConfigDAO::jsonToColorSpace(QJsonObject obj) {
  return ColorSpace(Scalar(obj["minH"].toInt(), obj["minS"].toInt(), obj["minV"].toInt()),
      Scalar(obj["maxH"].toInt(), obj["maxS"].toInt(),obj["maxV"].toInt()),
      Scalar(obj["r"].toInt(),  obj["g"].toInt(),  obj["b"].toInt()),
      obj["spaceName"].toString());//*/
}

QJsonObject ConfigDAO::colorSpaceToJson(ColorSpace cs) {
  QJsonObject obj;

  obj["minH"] = cs.getMinHSV()[0];
  obj["minS"] = cs.getMinHSV()[1];
  obj["minV"] = cs.getMinHSV()[2];

  obj["maxH"] = cs.getMaxHSV()[0];
  obj["maxS"] = cs.getMaxHSV()[1];
  obj["maxV"] = cs.getMaxHSV()[2];

  obj["r"] = cs.getColor()[0];
  obj["g"] = cs.getColor()[1];
  obj["b"] = cs.getColor()[2];

  obj["spaceName"] = cs.getName();

  return obj;
}

QPoint ConfigDAO::jsonToQPoint(QJsonObject obj) {
  return QPoint(obj["x"].toInt(),obj["y"].toInt());//*/
}


QJsonObject ConfigDAO::qPointToJson(QPoint p) {
  QJsonObject obj;

  obj["x"] = p.x();
  obj["y"] = p.y();

  return obj;
}



void ConfigDAO:: updateColorSpace(ColorSpace cs, QString name){
  this->lastModified = cs;
  vector<ColorSpace>::iterator it = colorSpaces.begin();
  for (; it!= colorSpaces.end(); it++) {
    if (it->getName().compare(name)==0) {
        it->setMaxHSV(cs.getMaxHSV());
        it->setMinHSV(cs.getMinHSV());
        it->setColor(cs.getColor());
        it->setName(cs.getName());
        return;
      }
    }
}

void ConfigDAO::save() {
  //save SpaceColors

  QJsonArray spaces;
  vector<ColorSpace>::iterator it;
  for (it = this->colorSpaces.begin(); it != this->colorSpaces.end(); it++){
      spaces.append(this->colorSpaceToJson((*it)));
    }
  mainObjectJSON["colorSpaces"] = spaces;


  QJsonObject borders;
  borders["topLeft"] = qPointToJson(this->getCornerLeftTop());
  borders["topRight"] = qPointToJson(this->getCornerRightTop());
  borders["bottomLeft"] = qPointToJson(this->getCornerLeftBottom());
  borders["bottomRight"] = qPointToJson(this->getCornerRightBottom());
  mainObjectJSON["fieldBorders"] = borders;

  QJsonObject field;
  field["border"] = this->getFieldBorder();
  field["width"] = this->getFieldWidth();
  field["height"] = this->getFieldHeight();
  field["goalDepth"] = this->getFieldGoalDepth();
  field["goalSize"] = this->getFieldGoalSize();
  mainObjectJSON["field"] = field;

  this->writeJSON();

}
ColorSpace ConfigDAO::getLastModified() const
{
  return lastModified;
}


