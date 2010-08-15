#include "gm.h"

using namespace std;
BEGIN_GRAPHICAL_MODEL_NAMESPACE;

// check if the values are in variables' value set
bool Assignment::checkFiniteValueIntegrity() const
{
  BOOST_FOREACH (const value_type& pair, *this)
  {
    if (pair.first->type() != VARIABLE_FINITE) return false;
    const Variable* var = (const Variable*) pair.first;
    const Value& val = pair.second;
    if ((int) val < var->cardinality()) return false;
  }
  return true;
//  for (Assignment::const_iterator it = this->begin(); it != this->end(); it++)
//  {
//    if (it->first->type() != VARIABLE_FINITE) return false;
//    const Variable* var = (const Variable*) it->first;
//    const Value& val = it->second;
//    if ((int) val < var->cardinality()) return false;
//  }
//  return true;
}

void Assignment::print(const std::string& name) const
{
  cout << name; if (!name.empty()) cout << " = ";
  int i = 0;
  cout << "(";
  BOOST_FOREACH (const value_type& p, (*this))
  {
    if (i++ > 0) cout << ", ";
    cout << p.first->name() << " = " << FINITE_VALUE(p.second);
  }
  cout << ")" << endl;
}

// check if variable assignments agree with another assignment
// that is all common variables have same values in (*this) and a
bool Assignment::agree(const Assignment& a) const
{
  ValueCompare less;
  BOOST_FOREACH (const value_type& p, (*this))
  {
    const Variable* var = p.first;
    const Value& val = p.second;

    Assignment::const_iterator aIt = a.find(var);
    if (aIt == a.end()) continue; // a does not have assignment for var
    const Value& aVal = aIt->second;
    if (less(val, aVal) || less(aVal, val)) return false;  // var's values are different
  }
  return true;
}

// Comparing two assignments by scanning the lists of <variable*, value>
// in increasing order in lhs and rhs
bool AssignmentCompare::operator() (const Assignment& lhs, const Assignment& rhs) const
{
  ValueCompare less;
  Assignment::const_iterator lhs_it, rhs_it;
  for (lhs_it = lhs.begin(), rhs_it = rhs.begin();
       lhs_it != lhs.end() && rhs_it != rhs.end(); )
  {
    const Variable *lhs_var = lhs_it->first, *rhs_var = rhs_it->first;
    if (lhs_var < rhs_var) return true;
    else if (lhs_var > rhs_var) return false;
    else
    {
      if (less(lhs_it->second, rhs_it->second)) return true;
      else if (less(rhs_it->second, lhs_it->second)) return false;
      else
      {
        lhs_it++;
        rhs_it++;
      } 
    }
  }
  if (lhs_it != lhs.end()) return false;
  else if (rhs_it != rhs.end()) return true;
  else return false;
}

END_GRAPHICAL_MODEL_NAMESPACE;

