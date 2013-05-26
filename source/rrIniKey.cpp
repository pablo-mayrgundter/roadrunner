#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include "rrIniKey.h"

using namespace rr;
namespace rr
{
rrIniKey::rrIniKey(const string& key)
{
    SetupKey(key);
}

void rrIniKey::ReKey(const string& key)
{
    SetupKey(key);
}

ostream&  operator<<(ostream& stream, const rrIniKey& aKey)
{
    stream<<aKey.AsString();
    return stream;
}

string rrIniKey::AsString() const
{
    string tmp = mKey;
    tmp += " = ";
    tmp += mValue;
    return tmp;
}

int rrIniKey::AsBool() const
{
    return toBool(mValue);
}

int rrIniKey::AsInt() const
{
    return toInt(mValue);
}

double rrIniKey::AsFloat() const
{
    return toDouble(mValue);
}

complex<double> rrIniKey::AsComplex() const
{
	vector<string> vals = splitString(mValue,",");
    if(vals.size() == 2)
    {
    	return complex<double>(toDouble(vals[0]), toDouble(vals[1]));
    }
    return complex<double>(toDouble(mValue), 0 );
}

void rrIniKey::SetupKey(const string& key)
{
    if(key.size())
    {
        vector<string> recs = splitString(key, "=");
        if(recs.size() > 0)
        {
            mKey = recs[0];
            mValue = recs[1];
          }
	    mComment 	= gEmptyString;
    }
    else
    {
        mKey 		= gEmptyString;
        mValue 		= gEmptyString;
        mComment 	= gEmptyString;
    }
}

}
