/**************************************************************************
 * This file is property of and copyright by the Experimental Nuclear     *
 * Physics Group, Dep. of Physics                                         *
 * University of Oslo, Norway, 2007                                       *
 *                                                                        *
 * Author: Per Thomas Hille <perthi@fys.uio.no> for the ALICE HLT Project.*
 * Contributors are mentioned in the code where appropriate.              *
 * Please report bugs to perthi@fys.uio.no                                *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

#include "AliHLTPHOSBase.h"
#include "AliHLTPHOSConfig.h"
#include "AliHLTPHOSAltroConfig.h"


AliHLTPHOSBase::AliHLTPHOSBase(): AliHLTPHOSConfig(),  
				  AliHLTPHOSAltroConfig()
{

}


AliHLTPHOSBase::~AliHLTPHOSBase()
{

}


bool
AliHLTPHOSBase::CheckFile(const char *fileName, const char *opt) const
{
  FILE *fp = fopen(fileName, opt);

  if(fp == 0)
    {
      cout <<"Error: from" << typeid(*this).name()  <<"could not open file: "<< fileName <<endl; 
      return false;
     }
  else
    {
      fclose(fp); 
      return true;
    }
}
