{
   gROOT->Reset();

//gStyle->SetOptLogx();
gStyle->SetOptLogy();
gStyle->SetOptFit(1011);
Char_t linha[10000];

    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile("iron/DAT4561514_tratado");

    while (std::getline(myfile, line))
        ++number_of_lines;
    std::cout << "Number of lines in text file: " << number_of_lines << "\n";
    return 0;


FILE *fp1 = fopen("iron/DAT4561514_tratado","r");

Int_t nlines=0;
Float_t part,x_p, y_p, r_p, w,d_r_p;

//num_lin = 20976079;
sqrt_lin = TMath::Sqrt(number_of_lines);
d_r_p = 10000/sqrt_lin;
//r_0 = 78;


c1 = new TCanvas("c1","Histograma unidimensional",10,10,700,700);

//c1->SetLogx(1);

hpx= new TH1F("hpx","Primary iron 4.64E15 eV",100,0,1000);

while (nlines<number_of_lines) {
	int ch;
	ch=fgetc(fp1);
	if (ch==EOF) break;

	ungetc(ch,fp1);
	fgets(linha,sizeof(linha),fp1);

     (void)sscanf(linha,"%f" "%f" "%f" "%f" , &part, &x_p, &y_p ,&r_p);
         //cout << r_p << endl;
         w = 1/(100*2*TMath::Pi()*r_p*d_r_p);
         //cout << TMath::Pi() << " " << r_p << " " << part << " "<< d_r_p << " " << w << endl;
    hpx->Fill(r_p,w);
    nlines++;
   }
c1->Update();
hpx->GetXaxis()->SetTitle("(cm)");
//c1->SetLogx();
hpx->Draw();

// Lateral distribution function - Nishimura-Kamata-Greisen (NKG)
//*TMath::Exp([4]*TMath::Log(1+0.35*(x/[3])))

TF1 *f = new TF1("fit", "([0]/([3]*[3]))*TMath::Exp(-[1]*TMath::Log(x/[3]))*TMath::Exp(-[2]*TMath::Log(1+(x/[3])))",0,1000000);
	
	f->SetParameters(300,-10,12,1000);
	//f->FixParameter(0,0.0006);
	//f->FixParameter(1,0.88);
	//f->FixParameter(2,1);
	f->SetParNames("C","a","b","r0");
	//hpx->Fit("fit");

// Creates a Root function
/* TF1 *f = new TF1("fit","[0]*TMath::Exp(-[1]*TMath::Log(x))",1,300);
	
 f->SetParameters(20000,2.7);
 f->SetParNames("Normal.","Inclinacao");

 hpx->Fit("fit");*/

}




