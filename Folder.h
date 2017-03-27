#ifndef FOLDER_H
#define FOLDER_H

#include <vector>
#include <memory>
#include "Shapeclip.h"

using namespace std;

class Folder {
public:
  Folder(const string& name, Folder* parent = nullptr);
  shared_ptr<Folder> create_child_folder(const string& name);  
  void push_shape( shared_ptr<Shapeclip> shape );  
  const vector< shared_ptr<Shapeclip> >& get_shapes() const ;  
  const vector< shared_ptr<Folder> >& get_folders() const ;  
  void draw();  
  void erase_shapes(bool &lock);
  void scale(double s, double x, double y);
  void moveBy(int dx, int dy);
  
  static void print(const shared_ptr<Folder>& folder, int x, int& y, const string& prefix = "");
private:
  Folder* parent;
  string name;
  vector< shared_ptr<Folder> > folders;
  vector< shared_ptr<Shapeclip> > shapes;
};

#endif
