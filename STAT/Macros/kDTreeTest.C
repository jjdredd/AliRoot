/*
  Test macro for TKDTree
  
  //Initialization:

  gSystem->AddIncludePath("-I$ALICE_ROOT/STAT")
  gSystem->Load("$ALICE_ROOT/lib/tgt_linux/libSTAT");
  .L $ALICE_ROOT/STAT/Macros/kDTreeTest.C+ 

  
  TestBuild();       // test build function of kdTree for memory leaks
  TestSpeed();       // test the CPU consumption to build kdTree
  TestkdtreeIF();    // test functionality of the kdTree
  TestSizeIF();      // test the size of kdtree - search application - Alice TPC tracker situation
  //
*/

#include <malloc.h>
#include "TSystem.h"
#include "TMatrixD.h"
#include "TRandom.h"
#include "TGraph.h"
#include "TStopwatch.h"
#include "TKDTree.h"




void TestBuild(const Int_t npoints = 1000000, const Int_t bsize = 100);
void TestSpeed(const Int_t npower2 = 20, const Int_t bsize = 10);

void TestkdtreeIF(Int_t npoints=1000, Int_t bsize=9, Int_t nloop=1000, Int_t mode = 2);
void TestSizeIF(Int_t nsec=36, Int_t nrows=159, Int_t npoints=1000,  Int_t bsize=10, Int_t mode=1);



Float_t Mem()
{
  // get mem info
  ProcInfo_t procInfo;
  gSystem->GetProcInfo(&procInfo);
  return procInfo.fMemVirtual;
}

//______________________________________________________________________
void kDTreeTest()
{
  //
  //
  //
  printf("\n\tTesting kDTree memory usage ...\n");
  TestBuild();  
  printf("\n\tTesting kDTree speed ...\n");
  TestSpeed();
}

//______________________________________________________________________
void TestBuild(const Int_t npoints, const Int_t bsize){  
  //
  // Test kdTree for memory leaks
  //
  Float_t *data0 =  new Float_t[npoints*2];
  Float_t *data[2];
  data[0] = &data0[0];
  data[1] = &data0[npoints];
  for (Int_t i=0;i<npoints;i++) {
    data[1][i]= gRandom->Rndm();
    data[0][i]= gRandom->Rndm();
  }
  Float_t before =Mem();
  TKDTreeIF *kdtree = new TKDTreeIF(npoints, 2, bsize, data);
	Float_t after = Mem();
  printf("Memory usage %f KB\n",after-before);
	delete kdtree;
  Float_t end = Mem();
  printf("Memory leak %f KB\n", end-before);
	return;	
}

//______________________________________________________________________
void TestSpeed(const Int_t npower2, const Int_t bsize)
{
  //
  // Test of building time of kdTree
  //
  if(npower2 < 10){
    printf("Please specify a power of 2 greater than 10\n");
    return;
  }
  
  Int_t npoints = Int_t(pow(2., npower2))*bsize;
  Float_t *data0 =  new Float_t[npoints*2];
  Float_t *data[2];
  data[0] = &data0[0];
  data[1] = &data0[npoints];
  for (Int_t i=0;i<npoints;i++) {
    data[1][i]= gRandom->Rndm();
    data[0][i]= gRandom->Rndm();
  }
  
  TGraph *g = new TGraph(npower2-10);
  g->SetMarkerStyle(7);
  TStopwatch timer;
  Int_t tpoints;
  TKDTreeIF *kdtree = 0x0;
  for(int i=10; i<npower2; i++){
    tpoints = Int_t(pow(2., i))*bsize;
    timer.Start(kTRUE);
    kdtree = new TKDTreeIF(tpoints, 2, bsize, data);
    timer.Stop();
    g->SetPoint(i-10, i, timer.CpuTime());
    printf("npoints [%d] nodes [%d] cpu time %f [s]\n", tpoints, kdtree->GetNNodes(), timer.CpuTime());
    //timer.Print("u");
    delete kdtree;
  }
  g->Draw("apl");
  return;
}

//______________________________________________________________________
void TestSizeIF(Int_t nsec, Int_t nrows, Int_t npoints,  Int_t bsize, Int_t mode)
{
  //
  // Test size to build kdtree
  //
  Float_t before =Mem();
  for (Int_t isec=0; isec<nsec;isec++)
    for (Int_t irow=0;irow<nrows;irow++){
      TestkdtreeIF(npoints,1,mode,bsize);
    }
  Float_t after = Mem();
  printf("Memory usage %f\n",after-before);
}




void  TestkdtreeIF(Int_t npoints, Int_t bsize, Int_t nloop, Int_t mode)
{
//
// Test speed and functionality of 2D kdtree.
// Input parametrs:
// npoints - number of data points
// bsize   - bucket size
// nloop   - number of loops
// mode    - tasks to be performed by the kdTree
//         - 0  : time building the tree
//

 
  Float_t rangey  = 100;
  Float_t rangez  = 100;
  Float_t drangey = 0.1;
  Float_t drangez = 0.1;

  //
  Float_t *data0 =  new Float_t[npoints*2];
  Float_t *data[2];
  data[0] = &data0[0];
  data[1] = &data0[npoints];
  Int_t i;   
  for (i=0; i<npoints; i++){
    data[0][i]          = gRandom->Uniform(-rangey, rangey);
    data[1][i]          = gRandom->Uniform(-rangez, rangez);
  }
  TStopwatch timer;
  
  // check time build
  printf("building kdTree ...\n");
  timer.Start(kTRUE);
  TKDTreeIF *kdtree = new TKDTreeIF(npoints, 2, bsize, data);
  timer.Stop();
  timer.Print();
  if(mode == 0) return;
  
  Float_t countern=0;
  Float_t counteriter  = 0;
  Float_t counterfound = 0;
  
  if (mode ==2){
    if (nloop) timer.Start(kTRUE);
    Int_t res[npoints];
    Int_t nfound = 0;
    for (Int_t kloop = 0;kloop<nloop;kloop++){
      if (kloop==0){
	counteriter = 0;
	counterfound= 0;
	countern    = 0;
      }
      for (Int_t i=0;i<npoints;i++){
	Float_t point[2]={data[0][i],data[1][i]};
	Float_t delta[2]={drangey,drangez};
	Int_t iter  =0;
	nfound =0;
	Int_t bnode =0;
	//kdtree->FindBNode(point,delta, bnode);
	//continue;
	kdtree->FindInRangeA(point,delta,res,nfound,iter,bnode);
	if (kloop==0){
	  //Bool_t isOK = kTRUE;
	  Bool_t isOK = kFALSE;
	  for (Int_t ipoint=0;ipoint<nfound;ipoint++)
	    if (res[ipoint]==i) isOK =kTRUE;
	  counteriter+=iter;
	  counterfound+=nfound;
	  if (isOK) {
	    countern++;
	  }else{
	    printf("Bug\n");
	  }
	}
      }
    }
    
    if (nloop){
      timer.Stop();
      timer.Print();
    }
  }
  delete [] data0;
  
  counteriter/=npoints;
  counterfound/=npoints;
  if (nloop) printf("Find nearest point:\t%f\t%f\t%f\n",countern, counteriter, counterfound);
}
