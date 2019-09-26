
/**
 *
 * toqutree (pa3)
 * significant modification of a quadtree .
 * toqutree.cpp
 * This file will be used for grading.
 *
 */

#include "toqutree.h"


toqutree::Node::Node(pair<int,int> ctr, int dim, HSLAPixel a)
	:center(ctr),dimension(dim),avg(a),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
	{}

toqutree::~toqutree(){
	clear(root);
}

toqutree::toqutree(const toqutree & other) {
	root = copy(other.root);
}


toqutree & toqutree::operator=(const toqutree & rhs){
	if (this != &rhs) {
		clear(root);
		root = copy(rhs.root);
	}
	return *this;
}

toqutree::toqutree(PNG & imIn, int k){ 

/* This constructor grabs the 2^k x 2^k sub-image centered */
/* in imIn and uses it to build a quadtree. It may assume  */
/* that imIn is large enough to contain an image of that size. */
    int x=imIn.width();
    int y=imIn.height();
     int d=pow(2,k);
    int xs=(x-d)/2;
    int ys=(y-d)/2;
   
//    pair<int,int>start=make_pair(xs,ys);
    PNG* newimg=new PNG(d,d);
    
    for(int i=0;i<d;i++){
        for(int j=0;j<d;j++){
            *newimg->getPixel(i,j)=*imIn.getPixel(i+xs,j+ys);
        }
    }
    root=buildTree(newimg,k);
    delete (newimg);
    newimg=NULL;
}

int toqutree::size() {
    return size(root);
}

int toqutree::size(Node* n) {
    int ans=0;
//    Node* curr;
//    curr=root;
    while(n->dimension!=0){
        ans++;
        ans=size(n->NW)+size(n->NE)+size(n->SE)+size(n->SW)+ans;
        
    }
    return ans;
}


toqutree::Node* toqutree::buildTree(PNG * im, int k) {

/* your code here */

// Note that you will want to practice careful memory use
// In this function. We pass the dynamically allocated image
// via pointer so that it may be released after it is used .
// similarly, at each level of the tree you will want to 
// declare a dynamically allocated stats object, and free it
// once you've used it to choose a split point, and calculate
// an average.

    
//    PNG img= *im;

//    Node* curr;
    pair<int,int> ctr;
    
    if(k==0){
       ctr=make_pair(0,0);
//        stats* data=new stats(*im);
//        HSLAPixel AVG=data->getAvg(ctr,ctr);
//        delete (data);
       
        Node* curr=new Node(ctr,0,*im->getPixel(0,0));
        return curr;
        
    }
    else if (k==1){
        ctr=make_pair(1,1);
        pair<int,int> p1=make_pair(0,0);
        pair<int,int> p2=make_pair(1,0);
        pair<int,int> p3=make_pair(0,1);
        pair<int,int> p4=make_pair(1,1);
        

        PNG* SEc=getChild(p4,1,im);
        PNG* SWc=getChild(p3,1,im);
        PNG* NWc=getChild(p1,1,im);
        PNG* NEc=getChild(p2,1,im);
        stats* data=new stats(*im);
        HSLAPixel AVG=data->getAvg(p1,p4);
        delete (data);
        data=NULL;
        Node* curr=new Node(ctr,1,AVG);
        curr->NW=buildTree(NWc,k-1);
        curr->NE=buildTree(NEc,k-1);
        curr->SW=buildTree(SWc,k-1);
        curr->SE=buildTree(SEc,k-1);
        delete (SEc);
//        SEc=NULL;
        delete (NEc);
//        NEc=NULL;
        delete (SWc);
//        SWc=NULL;
        delete (NWc);
//        NWc=NULL;
        return curr;
    }
//    else if(k>1){
    int d =pow(2,k-1);
//    PNG *center=new PNG(d,d);
//    for(int i=0;i<d;i++){
//        for(int j=0;j<d;j++){
//            *center->getPixel(i,j)=*im->getPixel(d/2+i,d/2+j);
//
//        }
//    }
        
        int dnew=pow(2,k);
        PNG* newimg=new PNG(2*im->width(),2*im->height());
        for(int i=0;i<dnew;i++){
            for(int j=0;j<dnew;j++){
                *newimg->getPixel(i,j)=*im->getPixel(i,j);
            }
        }
        for(int i=dnew;i<2*dnew;i++){
            for(int j=0;j<dnew;j++){
                 *newimg->getPixel(i,j)=*im->getPixel(i-dnew,j);
            }
        }
        for(int i=0;i<dnew;i++){
            for(int j=dnew;j<2*dnew;j++){
                *newimg->getPixel(i,j)=*im->getPixel(i,j-dnew);
            }
            
        }
        
        for(int i=dnew;i<2*dnew;i++){
            for(int j=dnew;j<2*dnew;j++){
                *newimg->getPixel(i,j)=*im->getPixel(i-dnew,j-dnew);
            }
        }
        
        
        
        
        double en;
        double minen=1.79768e+308 ;
        pair<int,int> sp;
        for(int i=0;i<d;i++){
            for(int j=0;j<d;j++){
                sp=make_pair(d/2+i,d/2+j);
                 pair<int,int> selr=make_pair(sp.first+d-1,sp.second+d-1);
                 pair<int,int> swlr=make_pair(selr.first+d,selr.second);
                 pair<int,int> nelr=make_pair(selr.first,selr.second+d);
                 pair<int,int> nwlr=make_pair(selr.first+d,selr.second+d);
                pair<int,int> swu=make_pair(sp.first+d,sp.second);
                pair<int,int> neu=make_pair(sp.first,sp.second+d);
                 pair<int,int> nwu=make_pair(sp.first+d,sp.second+d);
                stats* ndata=new stats(*newimg);
                en =getEntropy(ndata,ctr,selr)+getEntropy(ndata,swu,swlr)+getEntropy(ndata,neu,nelr)+getEntropy(ndata,nwu,nwlr);
                delete(ndata);
                if(en<minen){
                    minen=en;
                    ctr=sp;
                    
                }
            }
        }
//        ctr=getCentre(newimg,k);
        
        pair<int,int> ul=make_pair(0,0);
        pair<int,int> lr=make_pair(pow(2,k)-1,pow(2,k)-1);
        stats* data=new stats(*im);
        HSLAPixel AVG=data->getAvg(ul,lr);
        delete (data);
//        data=NULL;
        Node* curr=new Node(ctr,k,AVG);
        PNG* SEc=getChild(ctr,k,newimg);
        pair<int,int> swul=make_pair((ctr.first)+d,ctr.second);
        PNG* SWc=getChild(swul,k,newimg);
        pair<int,int> neul=make_pair(ctr.first,(ctr.second)+d);
        PNG* NEc=getChild(neul,k,newimg);
        pair<int,int> nwul=make_pair((ctr.first)+d,(ctr.second)+d);
        PNG* NWc=getChild(nwul,k,newimg);
    
//        newimg=NULL;
        curr->NW=buildTree(NWc,k-1);
        curr->NE=buildTree(NEc,k-1);
        curr->SW=buildTree(SWc,k-1);
        curr->SE=buildTree(SEc,k-1);
       
        delete (SEc);
//        SEc=NULL;
        
        delete (SWc);
//        SWc=NULL;
        delete (NEc);
//        NEc=NULL;
        delete (NWc);
//        NWc=NULL;
       delete (newimg);
        return curr;
        
//    }

   
   
    
   
        
}


double toqutree::getEntropy(stats* data,pair<int,int> ul, pair<int,int> lr){
    double result;
//    stats *data=new stats(*img);
    result=data->entropy(ul,lr);
//    delete (data);
//    data=NULL;
    return result;
    
    
}

//pair<int,int> toqutree::getCentre(PNG * im, int k){
//
//
//}

PNG* toqutree::getChild(pair<int,int>sp,int k,PNG* im){
    int d =pow(2,k-1);
    PNG* imc=new PNG(d,d);
    int x=sp.first;
    int y=sp.second;
    
    for(int i=0;i<d;i++){
        for(int j=0;j<d;j++){
            *imc->getPixel(i,j)=*im->getPixel(x+i,j+y);
        }
    }
    return imc;
   
    
}

PNG toqutree::buildPNG(Node*n){

    if(n->NE==NULL){
         PNG img(pow(2,n->dimension),pow(2,n->dimension));
//        int f=n->center.first;
//        int s=n->center.second;
        int s = pow(2,(n->dimension));
        for(int i=0;i<s;i++){
            for(int j=0;j<s;j++){
                *(img.getPixel(i,j))=n->avg;

            }
        }

        return img;

    }
    else{
        return combine(buildPNG(n->SE),buildPNG(n->SW),buildPNG(n->NE),buildPNG(n->NW),n->center);



    }
//    for(int i=0;i<d;i++){
//        for(int j=0;j<d;j++){
//            *img->getPixel(i,j)=*
//        }
//    }
}

PNG toqutree::combine(PNG se,PNG sw,PNG ne,PNG nw,pair<int,int> center){
//    if(se.width()==1){
//        PNG img(2,2);
//        *img.getPixel(0,0)=*nw.getPixel(0,0);
//        *img.getPixel(1,0)=*ne.getPixel(0,0);
//        *img.getPixel(0,1)=*sw.getPixel(0,0);
//        *img.getPixel(1,1)=*se.getPixel(0,0);
//        return img;
//    }
//
//    else{
        int d=2*(se.width());
        PNG* img=new PNG(d,d);
        PNG fin(d,d);
        for(int i=0;i<d/2;i++){
            for(int j=0;j<d/2;j++){
                *img->getPixel(i,j)=*se.getPixel(i,j);
            }
        }
        for(int i=d/2;i<d;i++){
            for(int j=0;j<d/2;j++){
                 *img->getPixel(i,j)=*sw.getPixel(i-d/2,j);
            }
        }
        for(int i=0;i<d/2;i++){
            for(int j=d/2;j<d;j++){
                *img->getPixel(i,j)=*ne.getPixel(i,j-d/2);
            }
        }
        for(int i=d/2;i<d;i++){
            for(int j=d/2;j<d;j++){
               *img->getPixel(i,j)=*nw.getPixel(i-d/2,j-d/2);
            }
        }

        for(int i=0;i<d;i++){
            for(int j=0;j<d;j++){
                *fin.getPixel((center.first+i)%d,(j+center.second)%d)=*img->getPixel(i,j);
            }
        }
            delete (img);
            return fin;
//    }
}
//pair<int,int>toqutree::findPixel(){
//    pair<int,int> p;
//
//    return p;
//}
//pair<int,int> toqutree::fixCoord(int x, int y, int k){
//
//    int maxdim = pow(2,k);
//    if(x >= maxdim){
//        x = x - maxdim;
//    }else if(x < 0){
//        printf("x: NEGATIVE ! HEY BUG SON!! GO FIX iT ! \n");
//    }
//
//    if(y >= maxdim){
//        y = y - maxdim;
//    }else if(y < 0 ){
//        printf("y: NEGATIVE ! HEY BUG SON!! GO FIX iT ! \n");
//    }
//
//    pair<int,int> c(x,y);
//    return c;
//
//}
//
//// k is large square's dim
//// given lr (p), get lr of square
//pair<int,int> toqutree::getCoordlr(pair<int,int>p, int k){
//    int lrx = p.first + pow(2,k-1)- 1;
//    int lry = p.second + pow(2,k-1) - 1;
//
//
//    return fixCoord(lrx, lry, k);
//}
//
//
//vector<pair<int,int>> toqutree::getSplitCoord(pair<int,int> SE, int k){
//    pair<int,int> SElr = getCoordlr(SE, k);
//    vector<pair<int,int>> temp(3);
//
//    //possible bug: dimesion
//
//    pair<int, int>  SW = fixCoord(SElr.first + 1, SE.second,k);
//    pair<int, int>  NE = fixCoord(SE.first, SElr.second + 1,k);
//    pair<int, int>  NW = fixCoord(SElr.first + 1, SElr.second + 1,k);
//
//    temp[0] = SW;
//    temp[1] = NE;
//    temp[2] = NW;
//
//    return temp;
//}
//
////Helper function for render
//void toqutree::putImage(PNG & toPutImg, PNG & oriImg, pair<int,int> p, int k){
//    int size = pow(2,k);
//    int subSize = pow(2,k-1);
//    for(int i = 0; i < subSize; i++){
//        for(int j = 0; j < subSize; j++){
//            *toPutImg.getPixel((i+p.first)%size,(j+p.second)%size) = *oriImg.getPixel(i,j);
//        }
//    }
//}


//void toqutree::render(PNG & img,Node * n){
//    if(n->NW == NULL){
//        int size = pow(2,(n->dimension));
//        for(int i = 0; i < size; i++){
//            for(int j = 0; j < size; j++){
//                *img.getPixel(i,j) = n->avg;
//            }
//        }
//        return;
//    }
//    int k = n->dimension;
//
//    int size = pow(2,k);
//    int subsize = pow(2,k-1);
//
//    PNG subImg(subsize,subsize);
//
//    pair<int,int> SEp = n->center;
//
//    vector<pair<int,int>> u = getSplitCoord(SEp, k);
//    pair<int, int> SWp = u[0];
//    pair<int, int> NEp = u[1];
//    pair<int, int> NWp = u[2];
//
//
//    render(subImg,n->SE);
//    putImage(img, subImg, SEp, k);
//
//    render(subImg,n->SW);
//    putImage(img, subImg, SWp, k);
//
//    render(subImg,n->NE);
//    putImage(img, subImg, NEp, k);
//
//    render(subImg,n->NW);
//    putImage(img, subImg, NWp, k);
//
//}
PNG toqutree::render(){
    
    // My algorithm for this problem included a helper function
    // that was analogous to Find in a BST, but it navigated the
    // quadtree, instead.
    
    /* your code here */
//    PNG img(pow(2,root->dimension),pow(2,root->dimension));
//    render(img,root);
    return buildPNG(root);
    
}

/* oops, i left the implementation of this one in the file! */
void toqutree::prune(double tol){

    prune(root,tol);
    

}

/* called by destructor and assignment operator*/

void toqutree::prune(Node*  nn, double tol){
//    if(nn->dimension!=0){
    if(withinTol(nn,nn->avg,tol)){
        clear(nn->NE);
        clear(nn->NW);
        clear(nn->SW);
        clear(nn->SE);
    }
    else{
        prune(nn->NE,tol);
         prune(nn->SE,tol);
         prune(nn->SW,tol);
         prune(nn->NW,tol);
    }
    }


bool toqutree:: withinTol(Node* n,HSLAPixel p,double tol){
   
    if(n->SW==NULL){
        
        return n->avg.dist(p)<=tol;
    }
    else{
        return withinTol(n->NE,p,tol) && withinTol(n->NW,p,tol) && withinTol(n->SE,p,tol)
        && withinTol(n->SW,p,tol);

        }
    

}
void toqutree::clear(Node * & curr){
/* your code here */
    if(curr!=NULL){
        clear(curr->NW);
        clear(curr->NE);
        clear(curr->SE);
        clear(curr->SW);
        delete (curr);
        curr=NULL;
    }
   
}

/* done */
/* called by assignment operator and copy constructor */
toqutree::Node * toqutree::copy(const Node * other) {
    if(other==NULL){
        return NULL;
    }
    Node* nn=new Node(other->center,other->dimension,other->avg);
        nn->NE=copy(other->NE);
        nn->NW=copy(other->NW);
        nn->SW=copy(other->SW);
        nn->SE=copy(other->SE);
        
    
    return  nn;
}



