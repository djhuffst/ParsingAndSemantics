/*
 * Copyright (C) Mohsen Zohrevandi, 2017
 *               Rida Bazzi 2019
 * Do not share this file with anyone
 */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include "lexer.h"
//#include <set>

using namespace std;

vector<string> terminals;
vector<string> nonTerminals;
vector<string> orderedNT;
vector<string> orderedT;

vector<string> generatingTokens;
vector<string> genNonTerminals;

vector<int> ruleNum;
vector<int> usefulRules;

bool inFirstSets = false;
bool inCase2 = false;

//vector<set<int> > firstSets;
//vector<set<int> > followSets;

LexicalAnalyzer lexer;
struct allFsets
{
  vector<struct Set *> allSets;
};

struct Set
{
  vector<string> members;
  string name;
};

struct allFsets * FirstSets = new struct allFsets();
struct allFsets * FollowSets = new struct allFsets();

struct grammarList
{
    vector<struct rules>  list;
};



struct rules
{
    vector<struct toke> tokens;
    int ruleNum;
};
    
struct toke
{
    Token thisToke;
    bool LHS;
};

struct grammarList * gList = new grammarList();
vector<struct toke> rule;


void syntax_error()
{
    cout << "SYNTAX ERROR !!!\n";
    exit(1);
}

Token expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

void Parse_Id_list(struct rules * gRule,int num)
{
    //cout<<"in parse id list"<<endl;
  if(gRule->tokens[num].thisToke.token_type != ID)
  {
    syntax_error();
  }

  if(gRule->tokens[num + 1].thisToke.token_type == HASH)
  {
    return;
  }
  else
  {
    Parse_Id_list(gRule,num + 1);
  }
  
}

//Parsing
void Parse_Right_hand_side(struct rules * gRule)
{
    //cout<<"in parse RHS"<<endl;
  if(gRule->tokens[2].thisToke.token_type == HASH)
  {
    return;
  }
  else
  {
    Parse_Id_list(gRule, 2);
  }

}

void Parse_Rule(struct rules * gRule)
{
    //cout<<"in parse rule "<<gRule->tokens[0].thisToke.lexeme<<endl;
    
    if(gRule->tokens[0].thisToke.token_type == DOUBLEHASH)
    {
      return;
    }
  else if(gRule->tokens[0].thisToke.token_type != ID)
  {
    syntax_error();
  }
  if(gRule->tokens[1].thisToke.token_type != ARROW)
  {
    syntax_error();
  }
  Parse_Right_hand_side(gRule);
  if(gRule->tokens[gRule->tokens.size()-1].thisToke.token_type != HASH)
  {
    syntax_error();
  }
}

//start parsing with num = 0, then recurses on all exisiting rules
void Parse_Rule_list(int num)
{
    //cout<<"in parse rule list"<<endl;
    Parse_Rule(&gList->list[num]);
    //cout<<"num: "<<num<<" size: "<<gList->list.size()<<endl;
    
    if((num + 1) == gList->list.size())
    {
      //cout<<"here"<<endl;
    if(gList -> list[num].tokens[0].thisToke.token_type != DOUBLEHASH)
    {
      syntax_error();
    }
    else{
        return;
    }
}
  else if(gList->list.size() > (num))
  {
    num++;
    Parse_Rule_list(num);
  }

}



Token peek()
{
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    return t;
}




void deleteDups()
{
    for(int i = 0; i < terminals.size(); i++)
    {
        for(int j = i+1; j < terminals.size(); j++)
        {
            if(terminals[i].compare(terminals[j]) == 0)
            {
                terminals.erase(terminals.begin() + j);
                deleteDups();
                return;
            }
        }
    }
    for(int i = 0; i < nonTerminals.size(); i++)
    {
        for(int j = i+1; j < nonTerminals.size(); j++)
        {
            if(nonTerminals[i].compare(nonTerminals[j]) == 0)
            {
                nonTerminals.erase(nonTerminals.begin() + j);
                deleteDups();
                return;
            }
        }
    }
    for(int i = 0; i < orderedNT.size(); i++)
    {
       for(int j = i+1; j < orderedNT.size(); j++)
       {
           if(orderedNT[i].compare(orderedNT[j]) == 0)
           {
               orderedNT.erase(orderedNT.begin() + j);
               deleteDups();
               return;
           }
       }
    }
    for(int i = 0; i < generatingTokens.size(); i++)
    {
       for(int j = i+1; j < generatingTokens.size(); j++)
       {
           if(generatingTokens[i].compare(generatingTokens[j]) == 0)
           {
               generatingTokens.erase(generatingTokens.begin() + j);
               deleteDups();
               return;
           }
       }
    }
    for(int i = 0; i < ruleNum.size(); i++)
    {
       for(int j = i+1; j < ruleNum.size(); j++)
       {
           if(ruleNum[i] == ruleNum[j])
           {
               ruleNum.erase(ruleNum.begin() + j);
               deleteDups();
               return;
           }
        }
    }
    for(int i = 0; i < usefulRules.size(); i++)
    {
       for(int j = i+1; j < usefulRules.size(); j++)
       {
           if(usefulRules[i] == usefulRules[j])
           {
               usefulRules.erase(usefulRules.begin() + j);
               deleteDups();
               return;
           }
        }
    }
    for(int i = 0; i < genNonTerminals.size(); i++)
    {
       for(int j = i+1; j < genNonTerminals.size(); j++)
       {
           if(genNonTerminals[i] == genNonTerminals[j])
           {
               genNonTerminals.erase(genNonTerminals.begin() + j);
               deleteDups();
               return;
           }
        }
    }
    for(int i = 0; i < orderedT.size(); i++)
    {
       for(int j = i+1; j < orderedT.size(); j++)
       {
           if(orderedT[i] == orderedT[j])
           {
               orderedT.erase(orderedT.begin() + j);
               deleteDups();
               return;
           }
        }
    }
      
}

int gcount = 1;
bool inGeneratingList(struct rules * tokeCheck, bool gramCheck)
{
  int count = 0;
  for(int i = 2; i < tokeCheck -> tokens.size(); i++)
  {
    for(int j = 0; j < generatingTokens.size(); j++)
    {
      if(tokeCheck -> tokens[i].thisToke.token_type == HASH)
      {
      }
      else if(tokeCheck -> tokens[i].thisToke.lexeme.compare(generatingTokens[j]) == 0)
      {
        //cout<<"generatingTokens.size(): "<<generatingTokens.size()<<" ";
        //cout<<"glist token "<<i<<": "<<tokeCheck -> tokens[i].thisToke.lexeme<<" ";
        //cout<<"genToke "<<j<<": "<<generatingTokens[j]<<" ";
        //cout<<tokeCheck -> tokens[i].thisToke.lexeme<<" = "<<generatingTokens[j]<<endl;
        count ++;
        deleteDups();
      }     
    }
  }
  //cout<<endl;
  
  if(count == tokeCheck -> tokens.size() - 3)
  {
    if(!gramCheck)
    {
      //cout<<"pushing back "<<tokeCheck -> ruleNum<<endl;
        ruleNum.push_back(tokeCheck -> ruleNum);
    }
    return true;
  }
  else
  {
    return false;
  }
}






bool isTerminal(string checkTerm)
{
    for(int i = 0; i < nonTerminals.size(); i++)
    {
        if(checkTerm.compare(nonTerminals[i]) == 0)
        {
            return false;
        }
    }
    return true;
}


void createTokenList()
{
  //find non-terminals
    for(int i = 0; i < gList -> list.size(); i++)
    {
        for(int j = 0; j < gList -> list[i].tokens.size(); j++)
        {
            if(gList ->list[i].tokens[j].LHS == true )
            {
                nonTerminals.push_back(gList -> list[i].tokens[j].thisToke.lexeme);
            }
        }
    }
     orderedT.push_back("#");
     orderedT.push_back("$");
    //find terminals
    for(int i = 0; i < gList -> list.size(); i++)
    {
       for(int j = 0; j < gList -> list[i].tokens.size(); j++)
       {
           if(isTerminal(gList -> list[i].tokens[j].thisToke.lexeme))
           {

                   if(!((gList ->list[i].tokens[j].thisToke.token_type == HASH) || (gList ->list[i].tokens[j].thisToke.token_type == DOUBLEHASH) || (gList ->list[i].tokens[j].thisToke.token_type == ARROW)))
                   {
                       terminals.push_back(gList -> list[i].tokens[j].thisToke.lexeme);
                        orderedT.push_back(gList -> list[i].tokens[j].thisToke.lexeme);

                   }
           }
           else
           {
               orderedNT.push_back(gList -> list[i].tokens[j].thisToke.lexeme);
           }
           /*else
           {
            orderedT.push_back(gList -> list[i].tokens[j].thisToke.lexeme);
           }*/
       }
    }
    //remove duplicates
    deleteDups();

}


void findUSyms(struct rules * thisRule)
{
  for(int i = 2; i < thisRule->tokens.size() - 1; i++)
  {
    if(!isTerminal(thisRule->tokens[i].thisToke.lexeme))
    {
      //cout<<thisRule->tokens[i].thisToke.lexeme<<" ";
      //cout<<thisRule->tokens[i].thisToke.lexeme<<endl;
      genNonTerminals.push_back(thisRule->tokens[i].thisToke.lexeme);
    }
  }

  deleteDups();
  //cout<<"genNonTerminals.size(): "<<genNonTerminals.size()<<endl;
}

bool isIntMember(vector<int> set, int member)
{
  for(int i = 0; i < set.size(); i++)
  {
    if(set[i] == member)
    {
      return true;
    }
  }
  return false;
}
 
//Task 2 helper function
void genUsefulSymbs()
{ 
  vector<string> symbols;

  //Checks to see if start token is in generating list.
  //If start is not generating then there are no useful symbols.
  bool startIsGenerating = false;
  //vector<int> startRuleIndices;

  for(int i = 0; i < ruleNum.size(); i++)
  {
    if(gList -> list[ruleNum[i]].tokens[0].thisToke.lexeme.compare(gList -> list[0].tokens[0].thisToke.lexeme) == 0)
    {
      //cout<<"ruleNum[i]: "<<ruleNum[i]<<endl;
      //startRuleIndices.push_back()
      usefulRules.push_back(ruleNum[i]);
      generatingTokens.push_back(gList -> list[0].tokens[0].thisToke.lexeme);
      startIsGenerating = true;
      
    }
  }
  if(!startIsGenerating)
  {
    return;
  }

  /*for(int i = 0; i < usefulRules.size(); i ++)
  {
    if()
  }*/

  
  //adds non-terminals that follow start to list of generating symbols
  sort(ruleNum.begin(), ruleNum.end());
  for(int i = 0; i < gList -> list.size(); i ++)
  {
    if(gList -> list[i].tokens[0].thisToke.lexeme.compare(gList -> list[0].tokens[0].thisToke.lexeme) == 0 && isIntMember(ruleNum, i))
    { 
      for(int j = 2; j < gList -> list[i].tokens.size() - 1; j++)
      {
        if(!isTerminal(gList -> list[i].tokens[j].thisToke.lexeme))
        {
          //cout<<gList -> list[ruleNum[0]].tokens[j].thisToke.lexeme<<" ";
          genNonTerminals.push_back(gList -> list[i].tokens[j].thisToke.lexeme);
        }
      }
    }
  }

  //adds remaining reachable non-terminals to list of generating symbols
  int count = 0;
  int genNtSize = 0;
  while(count < 15)
  {
    for(int i = 0; i < ruleNum.size(); i++)
    {
      for(int j = 0; j < genNonTerminals.size(); j++)
      {
        if(gList->list[ruleNum[i]].tokens[0].thisToke.lexeme.compare(genNonTerminals[j]) == 0)
        {
          findUSyms(&gList->list[ruleNum[i]]);
          //genNonTerminals.push_back(gList->list[ruleNum[i]].tokens[0].thisToke.lexeme);
        }
      }
    }
    count++;
  }

  for(int i = 0; i < ruleNum.size(); i++)
  {
    for(int j = 0; j < genNonTerminals.size(); j++)
    {
      if(gList->list[ruleNum[i]].tokens[0].thisToke.lexeme.compare(genNonTerminals[j]) == 0)
      {

        //cout<<"rule i: "<<ruleNum[i]<<endl;
        usefulRules.push_back(ruleNum[i]);
      }
    }
  }
  sort(usefulRules.begin(), usefulRules.end());
}

bool isMember(vector<string> thisSet, string id)
{
  for(int i = 0; i < thisSet.size(); i++)
  {
    if(thisSet[i].compare(id) == 0)
    {
      return true;
    }
  }
  return false;
}

int findFirstIndex(string id)
{
  for(int i = 0; i < FirstSets -> allSets.size(); i++)
  {
    if(FirstSets -> allSets[i]->name.compare(id) == 0)
    {
      return i;
    }
  }
  cout<<"error, invalid entry."<<endl;
  return 99;
}

int findFollowIndex(string id)
{
  for(int i = 0; i < FollowSets -> allSets.size(); i++)
  {
    if(FollowSets -> allSets[i]->name.compare(id) == 0)
    {
      return i;
    }
  }
  cout<<"error,  find follow sets invalid entry."<<endl;
  return 99;
}

void ProcessRuleIVandV(int index)
{
  bool nonTerminal = true;
  int thisSetIndex = 0;
  int count = 0;
  
    for(int i = 2; i < gList->list[index].tokens.size(); i++)
    {
      thisSetIndex = findFirstIndex(gList->list[index].tokens[0].thisToke.lexeme);
      if(isTerminal(gList->list[index].tokens[i].thisToke.lexeme) && (gList->list[index].tokens[i].thisToke.token_type != HASH))
      {
        if(!isMember(FirstSets -> allSets[thisSetIndex]->members,gList->list[index].tokens[i].thisToke.lexeme))
        {
          FirstSets -> allSets[thisSetIndex]->members.push_back(gList->list[index].tokens[i].thisToke.lexeme);            
          FirstSets -> allSets[thisSetIndex]->members.erase( unique( FirstSets ->allSets[thisSetIndex]->members.begin(), FirstSets ->allSets[thisSetIndex]->members.end() ), FirstSets ->allSets[thisSetIndex]->members.end() );

        }
        break;
      }
      else if(gList->list[index].tokens[i].thisToke.token_type != HASH)
      {
        int thisSetIndex2 = findFirstIndex(gList->list[index].tokens[i].thisToke.lexeme);
        count++;
        if(isMember(FirstSets -> allSets[thisSetIndex2]->members,"#"))
        {   
          for(int j = 0; j < FirstSets ->allSets[thisSetIndex2]->members.size(); j++)
          {
              if(!isMember(FirstSets -> allSets[thisSetIndex]->members,FirstSets ->allSets[thisSetIndex2]->members[j]) /*&& FirstSets ->allSets[thisSetIndex2]->members[j].compare("#") != 0*/)
            {
              if(i == gList->list[index].tokens.size() - 2 )
              {
                FirstSets -> allSets[thisSetIndex]->members.push_back(FirstSets ->allSets[thisSetIndex2]->members[j]);
              }
              else if(FirstSets ->allSets[thisSetIndex2]->members[j].compare("#") != 0)
              {
                FirstSets -> allSets[thisSetIndex]->members.push_back(FirstSets ->allSets[thisSetIndex2]->members[j]);
              }
            }
            FirstSets ->allSets[thisSetIndex]->members.erase( unique( FirstSets ->allSets[thisSetIndex]->members.begin(), FirstSets ->allSets[thisSetIndex]->members.end() ), FirstSets ->allSets[thisSetIndex]->members.end() );

          }
        }
        else if(!isMember(FirstSets ->allSets[thisSetIndex2]->members,"#"))
        {
          for(int k = 0; k < FirstSets ->allSets[thisSetIndex2]->members.size(); k++)
          {
            if(!isMember(FirstSets -> allSets[thisSetIndex]->members,FirstSets ->allSets[thisSetIndex2]->members[k]) )
            { 
      
                FirstSets -> allSets[thisSetIndex]->members.push_back(FirstSets ->allSets[thisSetIndex2]->members[k]);
            }
          }
          FirstSets -> allSets[thisSetIndex]->members.erase( unique( FirstSets ->allSets[thisSetIndex]->members.begin(), FirstSets ->allSets[thisSetIndex]->members.end() ), FirstSets ->allSets[thisSetIndex]->members.end() );

          break;
        }
      }
    }
    //rule.clear();
  //}
}


//Task 3 helper function
void addFirstMember(string setName, string member)
{
  int setIndex = 0;
  for(int i = 0; i < FirstSets -> allSets.size(); i++)
  {
    if(FirstSets -> allSets[i] -> name.compare(setName) == 0)
    {
      setIndex = i;
    }
  }
  FirstSets ->allSets[setIndex] -> members.push_back(member);
}

void printSet(vector<string> set)
{
  int count = 0;
  for(int i = 0; i < orderedT.size(); i++)
  {
    for(int j = 0; j < set.size(); j++)
    {
      if(orderedT[i].compare(set[j]) == 0)
      {
        if(count == set.size() - 1)
        {
          cout<<set[j];
        }
        else
        {
          count++;
          cout<<set[j]<<", ";
        }
      }
      
    }
  }
}



void addFirstToFollow(string followName, string firstName)
{
  int firstIndex = findFirstIndex(firstName);
  int followIndex = findFollowIndex(followName);
  for(int i = 0; i < FirstSets ->allSets[firstIndex]->members.size(); i++)
  {
    if(!isMember(FollowSets -> allSets[followIndex]->members,FirstSets ->allSets[firstIndex]->members[i]) && FirstSets ->allSets[firstIndex]->members[i].compare("#") != 0 )
    {
      //cout<<"Adding "<<FirstSets ->allSets[firstIndex]->members[i]<<" to follow set of "<<FollowSets -> allSets[followIndex]->name<<endl;
      FollowSets -> allSets[followIndex]->members.push_back(FirstSets ->allSets[firstIndex]->members[i]);
    }
  }
}

void addFollowToFollow(string followEnd, string followFirst)
{
  int followFirstIndex = findFollowIndex(followFirst);
  int followEndIndex = findFollowIndex(followEnd);
  int size = FollowSets ->allSets[followFirstIndex]->members.size();
  for(int i = 0; i < size; i++)
  {
    if(!isMember(FollowSets -> allSets[followEndIndex]->members,FollowSets ->allSets[followFirstIndex]->members[i]))
    {
      //cout<<"Adding "<<FollowSets ->allSets[followFirstIndex]->members[i]<<" to follow set of "<<FollowSets -> allSets[followEndIndex]->name<<endl;
      FollowSets -> allSets[followEndIndex]->members.push_back(FollowSets ->allSets[followFirstIndex]->members[i]);
    }
  }
}

void FollowRulesIIandIII()
{
  //bool change = true;
  //while(change)
  //{
  for(int i = 0; i < gList -> list.size()-1; i++)
  {
    //for(int j = gList -> list[i].tokens.size() - 2; j > 2; j--)
    //{
      if(!isTerminal(gList -> list[i].tokens[gList -> list[i].tokens.size() - 2].thisToke.lexeme))
      {
        addFollowToFollow(gList -> list[i].tokens[gList -> list[i].tokens.size() - 2].thisToke.lexeme, gList -> list[i].tokens[0].thisToke.lexeme);
      }
      //cout<<"adding here"<<endl;
    //}
  }


  //cout<<"\nnow working on rule III"<<endl;
  for(int i = 0; i < gList -> list.size()-1; i++)
  {
    for(int j = 2; j < gList -> list[i].tokens.size() - 2; j++)
    {
      //bool addFirst
      for(int k = gList -> list[i].tokens.size() - 2; k > j; k--)
      {
        if(!isTerminal(gList -> list[i].tokens[j].thisToke.lexeme) && !isTerminal(gList -> list[i].tokens[k].thisToke.lexeme))
        {
          int firstIndex = findFirstIndex(gList -> list[i].tokens[k].thisToke.lexeme);
          if(isMember(FirstSets ->allSets[firstIndex]->members,"#"))
          {
            if(k == j+1)
            {
              //cout<<"i: "<< i <<" j: "<<j<<" k: "<<k<<endl;
              //cout<<"adding here"<<endl;
              addFollowToFollow(gList->list[i].tokens[j].thisToke.lexeme, gList->list[i].tokens[0].thisToke.lexeme);
              //cout<<"past here"<<endl; 
            }
          /*if(!isTerminal(gList -> list[i].tokens[j].thisToke.lexeme) && !isTerminal(gList -> list[i].tokens[k].thisToke.lexeme))
          {

            int firstIndex = findFollowIndex(gList -> list[i].tokens[k].thisToke.lexeme);

            addFollowToFollow(gList -> list[i].tokens[j].thisToke.lexeme, gList -> list[i].tokens[k].thisToke.lexeme);
            */

            break;
          }
        }
        //else if()
        else
        {
          break;
        }

      }

    }
  }
}

void FollowRulesIVandV()
{
  for(int i = 0; i < gList -> list.size()-1; i++)
  {
    //cout<<"from i loop i: "<<i<<endl;
    //cout<<"rule one nt: "<<gList ->list[0].tokens[0].thisToke.lexeme<<endl;
    //cout<<"rule one index 2: "<<gList ->list[0].tokens[2].thisToke.lexeme<<endl;

    if((i == 0) && gList -> list[i].tokens.size() == 4)
    {
      
        //cout<<"before find index 1"<<endl;
        int followIndex = findFollowIndex(gList -> list[i].tokens[0].thisToke.lexeme);
        //cout<<"follow index: "<<followIndex<<endl;
        //cout<<"Adding $ to follow set of "<<gList -> list[i].tokens[0].thisToke.lexeme<<endl;
        FollowSets -> allSets[followIndex]->members.push_back("$");
      
    }

    for(int j = gList -> list[i].tokens.size() - 2; j > 2; j--)
    {
     // cout<<"size"<<endl;
      //int backCount = j - 2;
      int firstIndex;// = findFirstIndex(gList -> list[i].tokens[j].thisToke.lexeme);
      int followIndex;// = findFollowIndex(gList -> list[i].tokens[j-1].thisToke.lexeme);
     // cout<<"i: "<<i<<" j: "<<j<<" tokens size: "<<gList -> list[i].tokens.size() - 2<<endl;
      if((i == 0) && (j == gList -> list[i].tokens.size() - 2) )//&& !isTerminal(gList -> list[i].tokens[j].thisToke.lexeme))
      {
        //cout<<"before find index 1"<<endl;
        followIndex = findFollowIndex(gList -> list[i].tokens[0].thisToke.lexeme);
        //cout<<"follow index: "<<followIndex<<endl;
        //cout<<"Adding $ to follow set of "<<gList -> list[i].tokens[0].thisToke.lexeme<<endl;
        FollowSets -> allSets[followIndex]->members.push_back("$");
      }
      else if(!isTerminal(gList -> list[i].tokens[j-1].thisToke.lexeme) && !isTerminal(gList -> list[i].tokens[j].thisToke.lexeme))
      {
        //cout<<"before find index 2"<<endl;
        firstIndex = findFirstIndex(gList -> list[i].tokens[j].thisToke.lexeme);
        followIndex = findFollowIndex(gList -> list[i].tokens[j-1].thisToke.lexeme);
        //for(int k = 2; k <)
        addFirstToFollow(gList -> list[i].tokens[j-1].thisToke.lexeme, gList -> list[i].tokens[j].thisToke.lexeme);

      }
      else if(!isTerminal(gList -> list[i].tokens[j-1].thisToke.lexeme) && isTerminal(gList -> list[i].tokens[j].thisToke.lexeme))
      {
        //cout<<"before find index 3"<<endl;
        followIndex = findFollowIndex(gList -> list[i].tokens[j-1].thisToke.lexeme);
        //cout<<"Adding "<<gList -> list[i].tokens[j].thisToke.lexeme<<" to follow set of "<<gList -> list[i].tokens[j-1].thisToke.lexeme<<endl;
        if(!isMember(FollowSets -> allSets[followIndex]->members,gList -> list[i].tokens[j].thisToke.lexeme))
        {
          FollowSets -> allSets[followIndex]->members.push_back(gList -> list[i].tokens[j].thisToke.lexeme);
        }        
        //cout<<"here i: "<<i<< " j: "<<j<<endl;
      }

    }
    //cout<<"done with I loop: "<<i<<endl;
  }

  for(int i = 0; i < gList -> list.size()-1; i++)
  {
    for(int j = 2; j < gList -> list[i].tokens.size()-2; j++)
    {
      for(int k = j+1; k < gList -> list[i].tokens.size()-1; k++)
      {
        if(!isTerminal(gList -> list[i].tokens[j].thisToke.lexeme) && !isTerminal(gList -> list[i].tokens[k].thisToke.lexeme))
        {
          int firstIndex = findFirstIndex(gList -> list[i].tokens[k].thisToke.lexeme);
          //int followIndex = findFollowIndex(gList -> list[i].tokens[j].thisToke.lexeme);
          addFirstToFollow(gList -> list[i].tokens[j].thisToke.lexeme, gList -> list[i].tokens[k].thisToke.lexeme);
          if(!isMember(FirstSets ->allSets[firstIndex]->members,"#"))
          {
            break;
          }
        }
        else if(!isTerminal(gList -> list[i].tokens[j].thisToke.lexeme) && isTerminal(gList -> list[i].tokens[k].thisToke.lexeme))
        {
          //int firstIndex = findFirstIndex(gList -> list[i].tokens[k].thisToke.lexeme);
          int followIndex = findFollowIndex(gList -> list[i].tokens[j].thisToke.lexeme);

          if(!isMember(FollowSets ->allSets[followIndex]->members,gList -> list[i].tokens[k].thisToke.lexeme))
          {
            FollowSets ->allSets[followIndex]->members.push_back(gList -> list[i].tokens[k].thisToke.lexeme);
            
          }
          break;
        }
        else
        {
          break;
        }

      }

    }
  }
}

// read grammar
void ReadGrammar()
{
    struct rules * gRule = new struct rules();
    struct toke * thisToken = new struct toke();

    int ruleCount = 0;
    
    Token t = peek();
    while(t.token_type != DOUBLEHASH)
    {
        t = lexer.GetToken();
 
        thisToken -> thisToke = t;
        thisToken -> LHS = false;
        Token t1 = peek();
        if(t1.token_type == ARROW)
        {
            thisToken -> LHS = true;
        }
        
        gRule -> tokens.push_back(*thisToken);
 
        if((t.token_type == HASH) || (t.token_type == DOUBLEHASH))
        {
            gRule -> ruleNum = ruleCount;
            ruleCount++;
            gList -> list.push_back(*gRule);
            gRule -> tokens.clear();
        }
        
    }
    createTokenList();
}


// Task 1
void printTerminalsAndNoneTerminals()
{
    //print terminals
   for(int k = 0; k < terminals.size(); k++)
    {
        if(k == terminals.size() - 1)
        {
            cout<<terminals[k]<<"  ";
        }
        else
        {
            cout<<terminals[k]<<" ";
        }
    }
    
    //print non-terminals
    for(int l = 0; l < orderedNT.size(); l++)
    {
        if(l == orderedNT.size() - 1)
        {
            cout<<orderedNT[l];
        }
        else
        {
            cout<<orderedNT[l]<<" ";
        }
    }
    cout<<endl;
}

// Task 2
void RemoveUselessSymbols()
{
    bool change = true;
    int count = 0;

    //add terminals to generating tokens list
    for(int i = 0; i < terminals.size(); i++)
    {
      generatingTokens.push_back(terminals[i]);
    }
  

    //recurse though grammar rules adding non-terminals to
    //generatingTokens struct. LHS Elements are added when all 
    //RHS tokens are in generating Tokens list and while 
    //there is change in the previous pass.
    while(count < 15)
    {
 
      for(int j = 0; j < gList -> list.size(); j++)
      {
        if(gList -> list[j].tokens[2].thisToke.token_type == HASH)
        {
          ruleNum.push_back(j);
          generatingTokens.push_back(gList -> list[j].tokens[0].thisToke.lexeme);
        }
        else if(inGeneratingList(&gList -> list[j],false))
        {
          //cout<<"adding this to genTokes: "<<gList ->list[j].tokens[0].thisToke.lexeme<<endl;
          generatingTokens.push_back(gList -> list[j].tokens[0].thisToke.lexeme);
        }
      }
      count++;

    }

    //Take generating symbols and find generating grammar rules
    genUsefulSymbs();
 
    deleteDups();
    //cout<<"here"<<endl;
    if(inCase2)
    {
      for(int k = 0; k < gList -> list.size(); k++)
      {
        int sizeCount = 0;
        //cout<<usefulRules.size()<<endl;
        for(int l = 0; l < usefulRules.size(); l++)
        {
          if(gList -> list[k].ruleNum == usefulRules[l])
          {
            while(sizeCount < gList -> list[k].tokens.size())
            {
              if(sizeCount == 1)
              {
                cout<<"-> ";
                sizeCount++;
              }
              //else if(s)
              else
              {
                if((gList -> list[k].tokens[sizeCount].thisToke.token_type == HASH) && (gList -> list[k].tokens[sizeCount - 1].thisToke.token_type == ARROW))
                {
                  
                  cout<<"#";
                  sizeCount++;
                }
                else
                {
                  cout<<gList -> list[k].tokens[sizeCount].thisToke.lexeme<<" ";
                  sizeCount++;
                }
              }
            }
            cout<<endl;
          }
        }
      }
    }
}



// Task 3
void CalculateFirstSets()
{
    //Create Set structs for each non-terminal
    for(int i = 0; i < nonTerminals.size(); i++)
    {
      struct Set * thisSet = new struct Set();
      thisSet -> name = nonTerminals[i];
      FirstSets -> allSets.push_back(thisSet);
    }

    //Check the first element of each RHS to see if it is a terminal.
    //If yes, add to first set of LHS. 
    for(int i = 0; i < gList->list.size(); i++)
    {
      if(i == gList->list.size() - 1)
      {
        break;
      }
      else if(gList -> list[i].tokens[2].thisToke.token_type == HASH)
      {
        addFirstMember(gList -> list[i].tokens[0].thisToke.lexeme,"#");
      }
      else if(isTerminal(gList -> list[i].tokens[2].thisToke.lexeme))
      {
        int thisSetIndex = findFirstIndex(gList -> list[i].tokens[0].thisToke.lexeme);

        if(!isMember(FirstSets -> allSets[thisSetIndex]->members,gList->list[i].tokens[2].thisToke.lexeme))
        {
          addFirstMember(gList -> list[i].tokens[0].thisToke.lexeme, gList -> list[i].tokens[2].thisToke.lexeme);    
        }
      }
    }

    int loop = 0;
    while(loop <10)
    {
      for(int i = 0; i < gList -> list.size(); i++)
      {
        ProcessRuleIVandV(i);
      }
      loop++;
    }
    
    for(int i = 0; i < FirstSets -> allSets.size(); i++)
    {
      sort( FirstSets -> allSets[i]->members.begin(), FirstSets -> allSets[i]->members.end() );
    }

    vector<int> printOrder;
    for(int i = 0; i < orderedNT.size(); i++)
    {
      for(int j = 0; j < FirstSets -> allSets.size(); j++)
      {
        if(FirstSets -> allSets[j] -> name.compare(orderedNT[i]) == 0)
        {
          printOrder.push_back(j);
        }
      }
    }

    if(inFirstSets)
    {
      //print first sets
      for(int i = 0; i < printOrder.size(); i++)
      {
        cout<<"FIRST("<<FirstSets -> allSets[printOrder[i]] -> name<<") = { ";
        printSet(FirstSets -> allSets[printOrder[i]] -> members);
        cout<<" }"<<endl;
      }
    }
}

// Task 4
void CalculateFollowSets()
{
    //Calculate first sets to be used for follow set calculations
    CalculateFirstSets();

    //Create Set structs for each non-terminal
    for(int i = 0; i < nonTerminals.size(); i++)
    {
      struct Set * thisSet = new struct Set();
      thisSet -> name = nonTerminals[i];
      FollowSets -> allSets.push_back(thisSet);
    }

    FollowRulesIVandV();

    int loop = 0;
    while(loop <10)
    {
      for(int i = 0; i < gList -> list.size(); i++)
      {
        FollowRulesIIandIII();
      }
      loop++;
    }
    

    for(int i = 0; i < FollowSets -> allSets.size(); i++)
    {
      sort(FollowSets -> allSets[i]->members.begin(), FollowSets -> allSets[i]->members.end() );
    }

    vector<int> printOrder;
    for(int i = 0; i < orderedNT.size(); i++)
    {
      for(int j = 0; j < FirstSets -> allSets.size(); j++)
      {
        if(FollowSets -> allSets[j] -> name.compare(orderedNT[i]) == 0)
        {
          printOrder.push_back(j);
        }
      }
    }

     //print follow sets
    for(int i = 0; i < printOrder.size(); i++)
    {
      cout<<"FOLLOW("<<FollowSets -> allSets[printOrder[i]] -> name<<") = { ";
      printSet(FollowSets -> allSets[printOrder[i]] -> members);
      cout<<" }"<<endl;
    }
    
}

// Task 5
void CheckIfGrammarHasPredictiveParser()
{
  RemoveUselessSymbols();
  /*if(usefulRules.size()==0)
  {
    cout<<"NO"<<endl;
  }
  else */
  if((usefulRules.size() == gList->list.size()-1) || (usefulRules.size() == gList->list.size()-1))
  {
    cout<<"YES"<<endl;
  }
  else
  {
    cout<<"NO"<<endl;
  }
}
    
int main (int argc, char* argv[])
{
    int task;

    if (argc < 2)
    {
        cout << "Error: missing argument\n";
        return 1;
    }

    /*
       Note that by convention argv[0] is the name of your executable,
       and the first argument to your program is stored in argv[1]
     */

    task = atoi(argv[1]);

    
    ReadGrammar();  // Reads the input grammar from standard input
                    // and represent it internally in data structures
                    // ad described in project 2 presentation file
    
    Parse_Rule_list(0);

    switch (task) {
        case 1: printTerminalsAndNoneTerminals();
            break;

        case 2: 
            inCase2 = true;
            RemoveUselessSymbols();
            break;

        case 3: 
            inFirstSets = true;
            CalculateFirstSets();
            break;

        case 4: CalculateFollowSets();
            break;

        case 5: CheckIfGrammarHasPredictiveParser();
            break;

        default:
            cout << "Error: unrecognized task number " << task << "\n";
            break;
    }
    return 0;
}

