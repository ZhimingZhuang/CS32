void listAll(const Class* c, string path)  // two-parameter overload
{
    path += c->name();
    cout<<path<<endl;
    
    vector<Class*> subclasses = c->subclasses();
    if(subclasses.size() == 0){
        return;
    }
    
    path +=  "=>";
    for(vector<Class*>::const_iterator it = subclasses.begin(); it != subclasses.end(); it++){
        listAll((*it), path);
    }
}