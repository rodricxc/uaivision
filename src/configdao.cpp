#include "configdao.h"

ConfigDAO* ConfigDAO::object = NULL;

ConfigDAO::ConfigDAO(QObject *parent) : QObject(parent) {
  this->fileName = QString("../src/config/UaiVision.json");
  cout << "Starting dao with config file from: " << fileName.toStdString() << endl;
  this->readJSON();
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
  ColorSpace i;
  for (it = this->colorSpaces.begin(); it != this->colorSpaces.end(); it++){
      spaces.append(this->colorSpaceToJson((*it)));
    }
  mainObjectJSON["colorSpaces"] = spaces;

  this->writeJSON();

}
ColorSpace ConfigDAO::getLastModified() const
{
  return lastModified;
}


