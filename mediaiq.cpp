#include <iostream>
#include <string>
#include <deque>
#include <map>
#include <list>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

map <string,struct book *> book_id;//1st key is book id, 2nd key is address where that book information is stored
map <string,struct customer *> customer_id;//1st key is customer id, 2nd key is address where that customer information is stored
map <int,struct transaction *> transaction_id;//1st key is transaction id, 2nd key is address where that transaction information is stored


struct book{
    string bookid;
    float price;
    string bookname;
    int  soldcount;

};

struct customer{
    list<int> tid;
    string custid;
    int noOfVisits;
    int purchase;
};

struct transaction{
    int tid;
    string custid;
    string booklist;
    int totalamt;

};

bool comp1 (book b1,book b2)//comparison for sort function
{                           //on the basis of total  copies sold of a book in increasing order
    if(b1.soldcount>b2.soldcount)
        return 1;
    else if(b1.soldcount==b2.soldcount)
    {
        int c;
         c=b1.bookid.compare(b2.bookid);
         if(c>0)
            return 0;
         else
            return 1;
    }
    else
        return 0;
}

bool comp4 (book b1,book b2)//comparison for sort function
{                           //on the basis of total  copies sold of a book in decreasing order
    if(b1.soldcount>b2.soldcount)
        return 0;
    else if(b1.soldcount==b2.soldcount)
    {
        int c;
         c=b1.bookid.compare(b2.bookid);
         if(c>0)
            return 0;
         else
            return 1;
    }
    else
        return 1;
}

bool comp2 (customer c1,customer c2)//comparison for sort function
{                                  //on the basis of no of times customer purchased books
   if(c1.noOfVisits>c2.noOfVisits)
   {
       return 1;
   }
   else if(c1.noOfVisits==c2.noOfVisits){
         int c;
       c= c1.custid.compare(c2.custid);
    if(c>0)
            return 0;
         else
            return 1;
    }
   else
    return 0;
}

bool comp3 (transaction t1,transaction t2)//comparison for sort function
{                                         //on the basis of total amount paid in a particular transaction
   if(t1.totalamt>t2.totalamt)
   {
       return 1;
   }
   else if(t1.totalamt==t2.totalamt)
   {
    int c;
       c= t1.custid.compare(t2.custid);
    if(c>0)
            return 0;
         else
            return 1;
   }
   else
    return 0;
}

int main (int argc, char *argv[])
{
    deque <book> book_list;
    deque <customer> cust;
    deque <transaction> tran;

    deque <book>::iterator itbook;
    deque <customer>::iterator itcust;
    deque <transaction>::iterator ittran;

    float i,n,t=1,p,j,k,q,w=2,siz;
    string s,amt,tran_file_path,price_file_path,token,bid,d,custid,bookid;
    customer * c;
    int  tflag=0,pflag=0,rflag=0,dflag=0,cflag=0,option=0;
    char ch;
    int range;
    string cid;

    //cin>>price_file_path;
    while ((option = getopt(argc, argv,"t:p:r:d:c:")) != -1) {
        switch (option) {
            case 'p' :
                pflag=1;
        price_file_path=optarg;
        break;
            case 't':
               tflag=1;
        tran_file_path=optarg;
        break;
            case  'r':
               rflag=1;
               n=atoi(optarg) ;
               break;
            case 'd':
                dflag=1;
                range=atoi(optarg) ;
                break;
            case 'c':
                cflag=1;
                cid=optarg;
                break;
        }
    }


    if(tflag==0 || pflag==0){//if transaction list path or price list path is missing
            cout<<"file path is missing";
        exit(1);
    }

        if(dflag==0 && rflag==0 &&cflag==0){//if no arguments are provided for query
                cout<<"No argument is provided for query";
            exit(1);
        }

        if((cflag==0&&dflag==1)||(cflag==1&&dflag==0)){//if either customer id or discount range is missing
                cout<<"partial arguments  are provided";//if both are not present then no error
        exit(1);

        }

    if(pflag==1){
    ifstream myfile(price_file_path.c_str());//opening price list file

    if(myfile.is_open())
    {
        while( getline(myfile,bookid,',') )
    {
        book b;
        b.bookid=bookid;
        getline(myfile,amt,',');
        b.price = (float)atof(amt.c_str());
        getline(myfile,b.bookname);
        b.soldcount=0;
        book_list.push_front(b);
        itbook=book_list.begin();
        book_id[b.bookid]=&(*itbook);//putting value in hash map of books
    }
    }
    else
    {
        cout<<"price file could not be opened";
        exit(1);
    }

    myfile.close();
    //  cin>>tran_file_path;
    }

    if(tflag==1){
    ifstream file(tran_file_path.c_str());// opening transaction file

    if(file.is_open())
    {
        while( getline(file,custid,',') )
    {
        if(!customer_id[custid]){//check if customer is not present
        c=new customer();     //if customer is present then changes are made on that customer only
        c->purchase=0;
        }
        else
        c= customer_id[custid];
        transaction tt;
        c->custid=custid;
        tt.custid=custid;
        tt.tid=t;
        c->tid.push_back(t);
        t++;
        getline(file,token);
        istringstream ss(token);
        p=0;
        d="";
        while(getline(ss, bid,','))
        {
             d+=bid;
             d+=',';
             p+=book_id[bid]->price;
             book_id[bid]->soldcount+=1;
        }
        tt.booklist=d;
        tt.totalamt=p;
        c->purchase+=p;
        c->noOfVisits+=1;
        tran.push_back(tt);
        if(!customer_id[custid])
        cust.push_back(*c);
        ittran = tran.end();
        itcust = cust.end();
        if(!customer_id[custid])
        customer_id[c->custid]=&(*itcust)-1;//putting the value in customer hash map
        transaction_id[tt.tid]=&(*ittran)-1;//putting the value in transaction hash map
    }
    }
    else
    {
        cout<<"transaction file could not be opened";
        exit(1);
    }

    file.close();

    }

   if(rflag==1)
    {
        sort(cust.begin(),cust.end(),comp2);//sort the customers on the basis of no of times customer purchased books

        siz=cust.size();

        for(i=0,k=0;k<n && i<siz;i++,k++)
        {
            cout<<cust[i].custid<<" "<<cust[i].noOfVisits<<",";
             if(i!=siz-1 && cust[i].noOfVisits==cust[i+1].noOfVisits)
                        k--;
        }

        itcust = cust.begin();
        for(;itcust!=cust.end();itcust++)//after sorting hash map will have different address for given customer instead of its own object's address
        customer_id[(*itcust).custid]=&(*itcust);//this will point the given customer id to its own object's address

        cout<<endl;

        sort(tran.begin(),tran.end(),comp3);//sort the transaction on the basis of total amount paid in a particular transaction

        siz=tran.size();

        for(i=0,k=0;k<n && i<siz;i++,k++)
        {
            cout<<tran[i].custid<<" "<<tran[i].totalamt<<",";
            if(i!=siz-1 && tran[i].totalamt==tran[i+1].totalamt)
                        k--;
        }

        ittran = tran.begin();
        for(;ittran!=tran.end();ittran++)//after sorting hash map will have different address for given transaction instead of its own object's address
        transaction_id[(*ittran).tid]=&(*ittran);//this will point the given transaction id to its own object's address
        cout<<endl;

        siz=book_list.size();
        sort(book_list.begin(),book_list.end(),comp1);//sort the transaction on the basis of total copies sold of a book in increasing order


        for(i=0,k=0;i<siz && k<n  ;i++,k++)
        {
                     cout<<book_list[i].bookid<<" "<<book_list[i].soldcount<<",";
                     if(i!=siz-1 && book_list[i].soldcount==book_list[i+1].soldcount)
                        k--;
        }

        cout<<endl;

        sort(book_list.begin(),book_list.end(),comp4);//sort the transaction on the basis of total copies sold of a book in decreasing order
        for(i=0,k=0;i<siz && k<n  ;i++,k++)
        {
                     cout<<book_list[i].bookid<<" "<<book_list[i].soldcount<<",";
                     if(i!=siz-1 && book_list[i].soldcount==book_list[i+1].soldcount)
                        k--;
        }

        itbook = book_list.begin();
        for(;itbook!=book_list.end();itbook++)
        book_id[(*itbook).bookid]=&(*itbook);

    }

    if(dflag==1&&cflag==1)
    {
        customer *c;
        c = customer_id[cid];//pointing to the customer whose customer id is provided

        if(rflag ==1)//to correct the format
            cout<<endl;
        if(!c){
            cout<<"customer not present";
        return 0;
        }
        if(c->purchase>range)
        {
            cout<<1;
            return 0;
        }
        else
        {
            cout<<0;
            return  0;
        }
    }
    return 0;
}
