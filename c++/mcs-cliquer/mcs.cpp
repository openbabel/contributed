// Maximum Common Substructure calculator
// Created by Igor Filippov, 2011, igor.v.filippov@gmail.com
// Based on cliquer-1.21 and OpenBabel.
// See README.cliquer for more information.
// The key functions are mces_cliquer(OBMol,OBMol) and mcis_cliquer(OBMol,OBMol)
// Both take two OBMol arguments are return an OBMol
// corresponding to the MCES (or MCIS) of the two molecules.


#include <iostream>
#include <utility>
#include <set>
#include <boost/unordered_map.hpp>

#include <openbabel/mol.h>
#include <openbabel/obconversion.h>
#include <openbabel/obiter.h>
#include <openbabel/oberror.h>

extern "C" {
#include "cliquer.h"
}

using namespace OpenBabel;
using namespace std;

typedef pair<unsigned int,unsigned int> uv;
typedef set<uv> graph;
typedef boost::unordered_map<uv, graph> neighborhood;
typedef vector<graph> accumulator;

void run_cliquer(const graph &P, const neighborhood &N, graph &M);

void create_modular_product_mces(OBMol &mol1, OBMol &mol2,graph &V,neighborhood &N);
void create_modular_product_mcis(OBMol &mol1, OBMol &mol2,graph &V,neighborhood &N);
OBMol convert_mces_to_mol(const OBMol &mol1, const graph &M);
OBMol convert_mcis_to_mol(const OBMol &mol1, const OBMol &mol2, const graph &M);
OBMol mces_cliquer(OBMol mol1, OBMol mol2);
OBMol mcis_cliquer(OBMol mol1, OBMol mol2);

int main(int argc,char **argv)
{
  OBConversion obconv1,obconv2;
  OBMol mol1,mol2;

  obErrorLog.StopLogging();

 if(argc<3)
  {
	cout << "Usage: mcs moleculeA moleculeB"<<endl;
    return 1;
  }

  obconv1.SetInFormat(obconv1.FormatFromExt(argv[1]));
  obconv2.SetInFormat(obconv2.FormatFromExt(argv[2]));

  bool notatend1 = obconv1.ReadFile(&mol1,argv[1]);
  bool notatend2 = obconv2.ReadFile(&mol2,argv[2]);
  if (!notatend1 || !notatend2) exit(1);

  mol1.PerceiveBondOrders(); 
  unsigned int bonds1 = mol1.NumBonds();
  mol2.PerceiveBondOrders(); 
  unsigned int bonds2 = mol2.NumBonds();
  double max_achievable_sim = (double) min(bonds1,bonds2) / max(bonds1, bonds2);
   
  OBMol result = mces_cliquer(mol1,mol2);
  unsigned int bondsm = result.NumBonds();
  double similarity = (double) bondsm / (bonds1+bonds2-bondsm);

  OBConversion conv;
  conv.SetOutFormat("SMI");
  conv.Read(&result);
  string str = conv.WriteString(&result, true); 
  cout<<str<<" "<<similarity<<endl;
 
  return(0);
}

OBMol mces_cliquer(OBMol mol1, OBMol mol2)
{
  graph P,M;
  neighborhood N;
  create_modular_product_mces(mol1,mol2,P,N);  
  run_cliquer(P,N,M);
  OBMol result = convert_mces_to_mol(mol1, M);
  return(result);
}

OBMol mcis_cliquer(OBMol mol1, OBMol mol2)
{
  graph P,M;
  neighborhood N;
  create_modular_product_mcis(mol1,mol2,P,N);  
  run_cliquer(P,N,M);
  OBMol result = convert_mcis_to_mol(mol1, mol2, M);
  return(result);
}

void run_cliquer(const graph &P, const neighborhood &N, graph &M)
{
  if (P.size()>0)
    {
      graph_t *g = graph_new(P.size());
      
      boost::unordered_map<uv, int> uv_to_int;
      vector <uv> int_to_uv;
      int n=0;
      for (graph::iterator i = P.begin(); i != P.end(); i++)
	{
	  uv_to_int[*i] = n++;
	  int_to_uv.push_back(*i);
	}
    
      bool edge_exists = false;
      for (graph::iterator i = P.begin(); i != P.end(); i++)
	{
	  neighborhood::const_iterator ni=N.find(*i);
	  if (ni == N.end()) continue;
	  graph E=ni->second;
	  int first = uv_to_int[*i];
	  for (graph::iterator j = E.begin(); j != E.end(); j++)
	    {
	      int second = uv_to_int[*j];
	      if (second > first)
		{
		  GRAPH_ADD_EDGE(g,first,second);
		  edge_exists = true;
		}
	    }
	}
      if (edge_exists)
	{
	  set_t s=clique_find_single(g,0,0,TRUE,NULL);
	  for (int i=0; i<SET_MAX_SIZE(s); i++) 
	    if (SET_CONTAINS(s,i))   
	      {
		uv u = int_to_uv[i];
		M.insert(u);
	      }	
	  set_free(s);
	}
      graph_free(g);
    }
}

void create_modular_product_mces(OBMol &mol1, OBMol &mol2,graph &V,neighborhood &N)
{
 FOR_BONDS_OF_MOL(u1,mol1)
    {
      FOR_BONDS_OF_MOL(v1,mol2)
	{
	  V.insert(make_pair(u1->GetIdx(),v1->GetIdx()));
	  FOR_BONDS_OF_MOL(u2,mol1)
	    {
	      if (u2->GetIdx() > u1->GetIdx())
		{
		  FOR_BONDS_OF_MOL(v2,mol2)
		    {
		      if (v2->GetIdx() != v1->GetIdx())
			{
			  OBAtom* u1a = u1->GetBeginAtom();
			  OBAtom* u1b = u1->GetEndAtom();
			  OBAtom* u2a = u2->GetBeginAtom();
			  OBAtom* u2b = u2->GetEndAtom();
			  OBAtom* v1a = v1->GetBeginAtom();
			  OBAtom* v1b = v1->GetEndAtom();
			  OBAtom* v2a = v2->GetBeginAtom();
			  OBAtom* v2b = v2->GetEndAtom();

			  if (((u1->GetBondOrder() == v1->GetBondOrder() ||  (u1->IsAromatic() && v1->IsAromatic())) &&
			       (u2->GetBondOrder() == v2->GetBondOrder() ||  (u2->IsAromatic() && v2->IsAromatic()))) &&

			      ((u1a->GetAtomicNum() == v1a->GetAtomicNum() && u1b->GetAtomicNum() == v1b->GetAtomicNum() &&
				u1a->GetFormalCharge() == v1a->GetFormalCharge() && u1b->GetFormalCharge() == v1b->GetFormalCharge() &&
				u1a->GetIsotope() == v1a->GetIsotope() && u1b->GetIsotope() == v1b->GetIsotope() &&
				u2a->GetAtomicNum() == v2a->GetAtomicNum() && u2b->GetAtomicNum() == v2b->GetAtomicNum() &&
				u2a->GetFormalCharge() == v2a->GetFormalCharge() && u2b->GetFormalCharge() == v2b->GetFormalCharge() &&
				u2a->GetIsotope() == v2a->GetIsotope() && u2b->GetIsotope() == v2b->GetIsotope()                                             ) ||

			       (u1a->GetAtomicNum() == v1b->GetAtomicNum() && u1b->GetAtomicNum() == v1a->GetAtomicNum() &&
				u1a->GetFormalCharge() == v1b->GetFormalCharge() && u1b->GetFormalCharge() == v1a->GetFormalCharge() &&
				u1a->GetIsotope() == v1b->GetIsotope() && u1b->GetIsotope() == v1a->GetIsotope() &&
				u2a->GetAtomicNum() == v2a->GetAtomicNum() && u2b->GetAtomicNum() == v2b->GetAtomicNum() &&
				u2a->GetFormalCharge() == v2a->GetFormalCharge() && u2b->GetFormalCharge() == v2b->GetFormalCharge() &&
				u2a->GetIsotope() == v2a->GetIsotope() && u2b->GetIsotope() == v2b->GetIsotope()                                            ) ||

			       (u1a->GetAtomicNum() == v1a->GetAtomicNum() && u1b->GetAtomicNum() == v1b->GetAtomicNum() &&
				u1a->GetFormalCharge() == v1a->GetFormalCharge() && u1b->GetFormalCharge() == v1b->GetFormalCharge() &&
				u1a->GetIsotope() == v1a->GetIsotope() && u1b->GetIsotope() == v1b->GetIsotope()  &&
				u2a->GetAtomicNum() == v2b->GetAtomicNum() && u2b->GetAtomicNum() == v2a->GetAtomicNum() &&
				u2a->GetFormalCharge() == v2b->GetFormalCharge() && u2b->GetFormalCharge() == v2a->GetFormalCharge() &&
				u2a->GetIsotope() == v2b->GetIsotope() && u2b->GetIsotope() == v2a->GetIsotope()                                           ) ||

			       (u1a->GetAtomicNum() == v1b->GetAtomicNum() && u1b->GetAtomicNum() == v1a->GetAtomicNum() &&
				u1a->GetFormalCharge() == v1b->GetFormalCharge() && u1b->GetFormalCharge() == v1a->GetFormalCharge() &&
				u1a->GetIsotope() == v1b->GetIsotope() && u1b->GetIsotope() == v1a->GetIsotope() &&
				u2a->GetAtomicNum() == v2b->GetAtomicNum() && u2b->GetAtomicNum() == v2a->GetAtomicNum()  &&
				u2a->GetFormalCharge() == v2b->GetFormalCharge() && u2b->GetFormalCharge() == v2a->GetFormalCharge() &&
				u2a->GetIsotope() == v2b->GetIsotope() && u2b->GetIsotope() == v2a->GetIsotope()                                         ) ) &&

			      ((u1a->GetIdx() == u2a->GetIdx() && v1a->GetIdx() == v2a->GetIdx() && u1a->GetAtomicNum() == v1a->GetAtomicNum() 
				&& u1a->GetFormalCharge() == v1a->GetFormalCharge() && u1a->GetIsotope() == v1a->GetIsotope()) ||
			       (u1a->GetIdx() == u2a->GetIdx() && v1a->GetIdx() == v2b->GetIdx() && u1a->GetAtomicNum() == v1a->GetAtomicNum()
				&& u1a->GetFormalCharge() == v1a->GetFormalCharge() && u1a->GetIsotope() == v1a->GetIsotope()) ||
			       (u1a->GetIdx() == u2a->GetIdx() && v1b->GetIdx() == v2a->GetIdx() && u1a->GetAtomicNum() == v1b->GetAtomicNum()
				&& u1a->GetFormalCharge() == v1b->GetFormalCharge() && u1a->GetIsotope() == v1b->GetIsotope()) ||
			       (u1a->GetIdx() == u2a->GetIdx() && v1b->GetIdx() == v2b->GetIdx() && u1a->GetAtomicNum() == v1b->GetAtomicNum()
				&& u1a->GetFormalCharge() == v1b->GetFormalCharge() && u1a->GetIsotope() == v1b->GetIsotope()) ||

			       (u1a->GetIdx() == u2b->GetIdx() && v1a->GetIdx() == v2a->GetIdx() && u1a->GetAtomicNum() == v1a->GetAtomicNum()
				&& u1a->GetFormalCharge() == v1a->GetFormalCharge() && u1a->GetIsotope() == v1a->GetIsotope()) ||
			       (u1a->GetIdx() == u2b->GetIdx() && v1a->GetIdx() == v2b->GetIdx() && u1a->GetAtomicNum() == v1a->GetAtomicNum()
				&& u1a->GetFormalCharge() == v1a->GetFormalCharge() && u1a->GetIsotope() == v1a->GetIsotope()) ||
			       (u1a->GetIdx() == u2b->GetIdx() && v1b->GetIdx() == v2a->GetIdx() && u1a->GetAtomicNum() == v1b->GetAtomicNum()
				&& u1a->GetFormalCharge() == v1b->GetFormalCharge() && u1a->GetIsotope() == v1b->GetIsotope()) ||
			       (u1a->GetIdx() == u2b->GetIdx() && v1b->GetIdx() == v2b->GetIdx() && u1a->GetAtomicNum() == v1b->GetAtomicNum()
				&& u1a->GetFormalCharge() == v1b->GetFormalCharge() && u1a->GetIsotope() == v1b->GetIsotope()) ||

			       (u1b->GetIdx() == u2a->GetIdx() && v1a->GetIdx() == v2a->GetIdx() && u1b->GetAtomicNum() == v1a->GetAtomicNum()
				&& u1b->GetFormalCharge() == v1a->GetFormalCharge() && u1b->GetIsotope() == v1a->GetIsotope()) ||
			       (u1b->GetIdx() == u2a->GetIdx() && v1a->GetIdx() == v2b->GetIdx() && u1b->GetAtomicNum() == v1a->GetAtomicNum()
				&& u1b->GetFormalCharge() == v1a->GetFormalCharge() && u1b->GetIsotope() == v1a->GetIsotope()) ||
			       (u1b->GetIdx() == u2a->GetIdx() && v1b->GetIdx() == v2a->GetIdx() && u1b->GetAtomicNum() == v1b->GetAtomicNum()
				&& u1b->GetFormalCharge() == v1b->GetFormalCharge() && u1b->GetIsotope() == v1b->GetIsotope()) ||
			       (u1b->GetIdx() == u2a->GetIdx() && v1b->GetIdx() == v2b->GetIdx() && u1b->GetAtomicNum() == v1b->GetAtomicNum()
				&& u1b->GetFormalCharge() == v1b->GetFormalCharge() && u1b->GetIsotope() == v1b->GetIsotope()) ||

			       (u1b->GetIdx() == u2b->GetIdx() && v1a->GetIdx() == v2a->GetIdx() && u1b->GetAtomicNum() == v1a->GetAtomicNum()
				&& u1b->GetFormalCharge() == v1a->GetFormalCharge() && u1b->GetIsotope() == v1a->GetIsotope()) ||
			       (u1b->GetIdx() == u2b->GetIdx() && v1a->GetIdx() == v2b->GetIdx() && u1b->GetAtomicNum() == v1a->GetAtomicNum()
				&& u1b->GetFormalCharge() == v1a->GetFormalCharge() && u1b->GetIsotope() == v1a->GetIsotope()) ||
			       (u1b->GetIdx() == u2b->GetIdx() && v1b->GetIdx() == v2a->GetIdx() && u1b->GetAtomicNum() == v1b->GetAtomicNum()
				&& u1b->GetFormalCharge() == v1b->GetFormalCharge() && u1b->GetIsotope() == v1b->GetIsotope()) ||
			       (u1b->GetIdx() == u2b->GetIdx() && v1b->GetIdx() == v2b->GetIdx() && u1b->GetAtomicNum() == v1b->GetAtomicNum()
				&& u1b->GetFormalCharge() == v1b->GetFormalCharge() && u1b->GetIsotope() == v1b->GetIsotope()) ||
			       
			       (u1a->GetIdx() != u2a->GetIdx() && u1a->GetIdx() != u2b->GetIdx() && u1b->GetIdx() != u2a->GetIdx() && u1b->GetIdx() != u2b->GetIdx() &&
				v1a->GetIdx() != v2a->GetIdx() && v1a->GetIdx() != v2b->GetIdx() && v1b->GetIdx() != v2a->GetIdx() && v1b->GetIdx() != v2b->GetIdx() )))
			    {
			      graph n1=N[make_pair(u1->GetIdx(),v1->GetIdx())];
			      n1.insert(make_pair(u2->GetIdx(),v2->GetIdx()));
			      N[make_pair(u1->GetIdx(),v1->GetIdx())]=n1;
			      graph n2=N[make_pair(u2->GetIdx(),v2->GetIdx())];
			      n2.insert(make_pair(u1->GetIdx(),v1->GetIdx()));
			      N[make_pair(u2->GetIdx(),v2->GetIdx())]=n2;
			    }
			}
		    }
		}
	    }
	}
    }
}


void create_modular_product_mcis(OBMol &mol1, OBMol &mol2,graph &V,neighborhood &N)
{
  FOR_ATOMS_OF_MOL(u1,mol1)
    {
      FOR_ATOMS_OF_MOL(v1,mol2)
	{
	  V.insert(make_pair(u1->GetIdx(),v1->GetIdx()));
	  FOR_ATOMS_OF_MOL(u2,mol1)
	    {
	      if (u2->GetIdx() > u1->GetIdx())
		{
		  FOR_ATOMS_OF_MOL(v2,mol2)
		    {
		      if (v2->GetIdx() != v1->GetIdx())
			{
			  OBBond* u1u2 = mol1.GetBond(u1->GetIdx() ,u2->GetIdx());
			  OBBond* v1v2 = mol2.GetBond(v1->GetIdx() ,v2->GetIdx());
			  if ( u1->GetAtomicNum() == v1->GetAtomicNum() 
			       && u2->GetAtomicNum() == v2->GetAtomicNum() 
			       && u1->GetFormalCharge() == v1->GetFormalCharge() 
			       && u2->GetFormalCharge() == v2->GetFormalCharge()
			       && u1->GetIsotope() == v1->GetIsotope()
			       && u2->GetIsotope() == v2->GetIsotope()
			       && ((u1u2 != NULL && v1v2  != NULL 
				    && (u1u2->GetBondOrder() == v1v2->GetBondOrder() 
					||  (u1u2->IsAromatic() && v1v2->IsAromatic())))
				   || (u1u2 == NULL && v1v2  == NULL)))
			    {
			      graph n1=N[make_pair(u1->GetIdx(),v1->GetIdx())];
			      n1.insert(make_pair(u2->GetIdx(),v2->GetIdx()));
			      N[make_pair(u1->GetIdx(),v1->GetIdx())]=n1;
			      graph n2=N[make_pair(u2->GetIdx(),v2->GetIdx())];
			      n2.insert(make_pair(u1->GetIdx(),v1->GetIdx()));
			      N[make_pair(u2->GetIdx(),v2->GetIdx())]=n2;
			    }
			}
		    }
		}
	    }
	}
    }
}


OBMol convert_mces_to_mol(const OBMol &mol1, const graph &M)
{
   OBMol out;
   out.SetDimension(2);
   out.BeginModify();
   vector<unsigned int> atom_index(mol1.NumAtoms()+1,0);
   unsigned int count = 1;

   for (graph::const_iterator i = M.begin(); i != M.end(); i++)
     {
       OBBond* b = mol1.GetBond(i->first);
       OBAtom* a1 = b->GetBeginAtom();
       OBAtom* a2 = b->GetEndAtom();

       if (atom_index[a1->GetIdx()] == 0)
	 {
	   OBAtom *n=out.CreateAtom();
	   n->SetAtomicNum(a1->GetAtomicNum());
	   n->SetFormalCharge(a1->GetFormalCharge());
	   n->SetIsotope(a1->GetIsotope());
	   n->SetVector(a1->x(),a1->y(),a1->z());
	   out.AddAtom(*n);
	   atom_index[a1->GetIdx()] = count++;
	   delete n;
	 }
       if (atom_index[a2->GetIdx()] == 0)
	 {
	   OBAtom *n=out.CreateAtom();
	   n->SetAtomicNum(a2->GetAtomicNum());
	   n->SetFormalCharge(a2->GetFormalCharge());
	   n->SetIsotope(a2->GetIsotope());
	   n->SetVector(a2->x(),a2->y(),a2->z());
	   out.AddAtom(*n);
	   atom_index[a2->GetIdx()] = count++;
	   delete n;
	 }
       out.AddBond(atom_index[a1->GetIdx()],atom_index[a2->GetIdx()], b->GetBondOrder(), b->GetFlags());
     }

   out.EndModify();
   return(out);
}

OBMol convert_mcis_to_mol(const OBMol &mol1, const OBMol &mol2, const graph &M)
{
  OBMol out;
  out.SetDimension(2);
  out.BeginModify();
  vector<unsigned int> atom_index(mol1.NumAtoms()+1,0);
  unsigned int count = 1;

  for (graph::iterator i = M.begin(); i != M.end(); i++)
    {
      OBAtom* a = mol1.GetAtom(i->first);
      OBAtom *n=out.CreateAtom();
      n->SetAtomicNum(a->GetAtomicNum());
      n->SetFormalCharge(a->GetFormalCharge());
      n->SetIsotope(a->GetIsotope());
      n->SetVector(a->x(),a->y(),a->z());
      out.AddAtom(*n);
      atom_index[i->first] = count++;
      delete n;
    }

  for (graph::iterator i = M.begin(); i != M.end(); i++)
    for (graph::iterator j = M.begin(); j != M.end(); j++)
      if (j->first > i->first)
	{
	  OBBond *ab = mol1.GetBond(i->first,j->first);
	  OBBond *cd = mol2.GetBond(i->second,j->second);
	  OBBond *nm = out.GetBond(atom_index[i->first],atom_index[j->first]);
	  if (ab != NULL && cd != NULL && nm == NULL && (ab->GetBondOrder() == cd->GetBondOrder() 
							 ||  (ab->IsAromatic() && cd->IsAromatic())))
	    {
	      out.AddBond(atom_index[i->first],atom_index[j->first], ab->GetBondOrder(), ab->GetFlags());
	    }
	}
  out.EndModify();
  return(out);
}
