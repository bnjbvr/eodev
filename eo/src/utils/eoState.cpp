
#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <algorithm>
#include <fstream>

#ifdef HAVE_SSTREAM
#include <sstream>
#else
#include <strstream>
#endif

#include "eoState.h"
#include "eoObject.h"
#include "eoPersistent.h"

using namespace std;



void removeComment(string& str, string comment)
{
    string::size_type pos = str.find(comment);

    if (pos != string::npos)
    {
        str.erase(pos, str.size());
    }
}

bool is_section(const string& str, string& name) 
{
    string::size_type pos = str.find("\\section{");

    if (pos == string::npos)
        return false;
    //else

    string::size_type end = str.find("}");

    if (end == string::npos)
        return false;
    // else

    name = str.substr(pos + 9, end-9);

    return true;
}

eoState::~eoState(void)
{
    for (unsigned i = 0; i < ownedObjects.size(); ++i)
    {
        delete ownedObjects[i];
    }
}
    
void eoState::registerObject(eoPersistent& registrant)
{
    string name = createObjectName(dynamic_cast<eoObject*>(&registrant));
 
    pair<ObjectMap::iterator,bool> res = objectMap.insert(make_pair(name, &registrant)); 

    if (res.second == true)
    {
        creationOrder.push_back(res.first);
    }
    else
    {
        throw logic_error("Interval error: object already present in the state");
    }
}

void eoState::load(const string& _filename)
{
    ifstream is (_filename.c_str());

    if (!is)
    {
        string str = "Could not open file " + _filename;
        throw runtime_error(str);
    }
    
    load(is);
}

void eoState::load(std::istream& is)
{
    string str;
    string name;

    getline(is, str);
    
    if (is.fail())
    {
        string str = "Error while reading stream";
        throw runtime_error(str);
    }
    
    while(is)
    { // parse section header
        if (is_section(str, name))
        {
            string fullString;
            ObjectMap::iterator it = objectMap.find(name);

            if (it == objectMap.end())
            { // ignore
                while (getline(is, str))
                {
                    if (is_section(str, name))
                        break;
                }
            }
            else
            {
            
                eoPersistent* object = it->second;

                // now we have the object, get lines, remove comments etc.

                string fullstring;

                while (getline(is, str))
                {
                    if (is_section(str, name))
                        break;

                    removeComment(str, getCommentString());
                    fullstring += str + "\n";
                }
#ifdef HAVE_SSTREAM
		istringstream the_stream(fullstring);
#else
                istrstream the_stream(fullstring.c_str(), fullstring.size());
#endif
                object->readFrom(the_stream);
            }
        }
    }

}

void eoState::save(const string& filename) const
{ // saves in order of insertion
    ofstream os(filename.c_str());

    if (!os)
    {
        string msg = "Could not open file: " + filename + " for writing!";
        throw runtime_error(msg);
    }

    save(os);
}

void eoState::save(std::ostream& os) const
{ // saves in order of insertion
    for (vector<ObjectMap::iterator>::const_iterator it = creationOrder.begin(); it != creationOrder.end(); ++it)
    {
        os << "\\section{" << (*it)->first << "}\n";
        (*it)->second->printOn(os);
        os << '\n';
    }
}

string eoState::createObjectName(eoObject* obj)
{
    if (obj == 0)
    {
#ifdef HAVE_SSTREAM
	ostringstream os;
#else
        ostrstream os;
#endif
        os << objectMap.size();
        return os.str();
    }
    // else
    
    string name = obj->className();
    ObjectMap::const_iterator it = objectMap.find(name);

    unsigned count = 1;
    while (it != objectMap.end())
    {
#ifdef HAVE_SSTREAM
	ostringstream os;
        os << obj->className().c_str() << count++;
#else
        ostrstream os;
        os << obj->className().c_str() << count++ << ends;
#endif

        name = os.str();

        it = objectMap.find(name);
    }

    return name;
}

