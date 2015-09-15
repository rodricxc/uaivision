#include "dbscam.h"


set<CalibPoint> DBSCAN::getPoints() const
{
    return points;
}

int DBSCAN::getMinPoints() const {
  return minPoints;
}

void DBSCAN::setMinPoints(int value) {
  minPoints = value;
}

double DBSCAN::getRaio() const {
  return raio;
}

void DBSCAN::setRaio(double value) {
  raio = value;
}

int DBSCAN::numOfNeighbors(CalibPoint *p) {
  set<CalibPoint >::iterator it = this->points.begin();
  int count = 0;
  for (;it != this->points.end(); ++it) {
    if (distancePixels(it->getHsv(),p->getHsv()) <= this->raio) {
        count++;
    }
  }
  return count;
}

void DBSCAN::insertToAnalise(CalibPoint *p) {
  this->toAnalise.insert(this->toAnalise.end(), p);
}

void DBSCAN::insertOutliers(CalibPoint *p) {
  this->outliers.insert(this->outliers.end(), p);
}

void DBSCAN::analise() {
  vector<CalibPoint*>::iterator i = this->toAnalise.begin();
  set<CalibPoint >::iterator j = this->points.begin();
  //set<CalibPoint*>::iterator k = this->points.begin();
  vector<CalibPoint*> cluster;
  while (!toAnalise.empty()) {
      i = this->toAnalise.begin();
    if ((*i)->isUndefined()) {
      int num = this->numOfNeighbors((*i) );
      if (num > this->minPoints) {
        (*i)->toCore();
      } else {
        (*i)->toOutlier();
      }
    }
    toAnalise.erase(toAnalise.begin());
  }
}



DBSCAN::DBSCAN() {
  this->setRaio(5);
  this->setMinPoints(10);
}

void DBSCAN::insertPoint(CalibPoint p) {
 /* set<CalibPoint>::iterator it = this->points.end();
  this->points.insert( this->points.end(), p);
  for (;it != this->points.end(); ++it) {
      insertToAnalise((CalibPoint*) &(*it));
    }
 */
}

void DBSCAN::insertPointsVector(vector<CalibPoint> v) {
  /*vector<CalibPoint>::iterator it = v.begin();
  pair<set<CalibPoint>::iterator,bool> ret;

  for (;it != v.end(); ++it) {
    ret = points.insert(*it);

    if (ret.second) {
      insertToAnalise((CalibPoint*) &(*it));
    }
  }
  //this->analise();
  cout << "tamanho   : " << points.size() << endl;
  cout << "tamanho an: " << toAnalise.size() << endl;
*/
}

void DBSCAN::clear() {
  this->toAnalise.clear();
  this->clusters.clear();
  this->outliers.clear();
  this->points.clear();
}

double DBSCAN::distancePixels(Scalar a, Scalar b) {
  return sqrt((a.val[0]-b.val[0])*(a.val[0]-b.val[0])+
      (a.val[1]-b.val[1])*(a.val[1]-b.val[1])+
      (a.val[2]-b.val[2])*(a.val[2]-b.val[2]));
}

