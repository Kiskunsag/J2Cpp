#include "registration.h"


using namespace std;
// Qt includes
#include <QHash>

//std includes
#include <vector>
#include <algorithm>


namespace{
    /// @todo not efficient. Work with hashes for faster lookup here? Alternativly: "As others have said, use the STL find or find_if functions. But if you are searching in very large vectors and this impacts performance, you may want to sort your vector and then use the binary_search, lower_bound, or upper_bound algorithms." (http://stackoverflow.com/questions/571394/how-to-find-an-item-in-a-stdvector). Or use a low-count (30) cache-vector as well. Alphabetical sorting might also help.
    vector<string> types;
}

bool isKnownType(const Token & token){
    return find(types.begin(), types.end(), token) != types.end();
}

bool registerType(const Token & token){
    if(isKnownType(token) || token.kind != tkIdentifier)
        return false;
    else
    {
        types.push_back(token.token);
        return true;
    }
}
