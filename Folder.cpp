#include "Folder.h"

Folder::Folder(const string& n, Folder* p){
  name = n;
  parent = p;
}


shared_ptr<Folder> Folder::create_child_folder(const string& name){
  folders.emplace_back( new Folder(name, this) );
  return folders.back();
}

void Folder::push_shape( shared_ptr<Shapeclip> shape ){
  shapes.push_back( shape );
}

const vector< shared_ptr<Shapeclip> >& Folder::get_shapes() const {
  return shapes;
}

const vector< shared_ptr<Folder> >& Folder::get_folders() const {
  return folders;
}

void Folder::draw(){
  for( auto& it:shapes ){
    it->draw();
  }

  for( auto& it:folders ){
    it->draw();
  }
}

void Folder::erase_shapes(bool &lock){
  while (lock) {}
  lock = true;
  
  bool childLock = false;
  for( auto& it:get_shapes() ){
    it->erase();
  }
  
  for( auto& it:get_folders() ){
    it->erase_shapes(childLock);
  }

  lock = false;  
}

void Folder::scale(double s, double x, double y){
  for( auto& it:shapes ){
    it->scale(s,x,y);
  }
  
  for( auto& it:folders ){
    it->scale(s,x,y);
  }
}

void Folder::moveBy(int dx, int dy){
  for( auto& it:shapes ){
    it->moveBy(dx,dy);
  }
  
  for( auto& it:folders ){
    it->moveBy(dx,dy);
  }
}

void SetCursorPos(int XPos, int YPos)
{
  printf("\033[%d;%dH", YPos+1, XPos+1);
}


void Folder::print(const shared_ptr<Folder>& folder, int x, int& y, const string& prefix)
{

  int i = 0;
  for( auto& it:folder->get_shapes() ){
    SetCursorPos( x, y++ );
    cout << prefix << "Shape_" << i << " ";
    ++i;
  }
  
  for( auto& it:folder->get_folders() ){
    SetCursorPos( x, y++ );
    cout << prefix << it->name  << " ";
    print( it,x,y, prefix + "-" );
  }
}