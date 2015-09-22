#include "configdao.h"

ConfigDAO* ConfigDAO::object = NULL;

ConfigDAO::ConfigDAO(QObject *parent) : QObject(parent) {
  this->fileName = QString("../src/config/UaiVision.json");
  cout << "Starting dao with config file from: " << fileName.toStdString() << endl;

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

      cout << "num of color spaces: " << colorSpaces.size() << endl;

    } else {
      cout << "ERROR!!!" << endl;
      return false;
    }

  return true;

}

bool ConfigDAO::writeJSON() {
  QFile f("../src/config/teste.json");
  QJsonDocument d;
  d.setObject(this->mainObjectJSON);
  f.open(QFile::WriteOnly);
  f.write(d.toJson());
  f.close();

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
      Scalar(obj["H"].toInt(),  obj["S"].toInt(),  obj["V"].toInt()),
      obj["spaceName"].toString());//*/
}


