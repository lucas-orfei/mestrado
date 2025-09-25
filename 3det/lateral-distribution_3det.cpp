{
   gROOT->Reset();

//gStyle->SetOptLogx();
gStyle->SetOptLogy();
gStyle->SetOptStat(10);
gStyle->SetOptFit(1);
Char_t linha[10000];

    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile("./iron/OUT456152_tratado");

    while (std::getline(myfile, line))
        ++number_of_lines;
    //std::cout << "Numero de linhas: " << number_of_lines << "\n";
    //return 0;


FILE *fp1 = fopen("./iron/OUT456152_tratado","r");

Int_t nlines=0,npart=0;
Float_t eas,part,x_p, y_p, r_p,w,d_r_p;

//Float_t xdet1=0,ydet1=346;
//Float_t xdet2=-400,ydet2=-346;
//Float_t xdet3=400,ydet3=-346;

Float_t r1,r2,r3,rdet=65;

//sqrt_lin = TMath::Sqrt(number_of_lines);
d_r_p = 10; //largura do bin
//r_0 = 78;


c1 = new TCanvas("c1","Histograma unidimensional",10,10,700,700);
//c2 = new TCanvas("c2","Histograma bidimensional",10,10,700,700);
c1->SetLeftMargin(0.15);

//c1->SetLogx(1);

//hpx= new TH1F("hpx","Primary photon 1E14 eV",100,0,10000);

hpx= new TH1F("hpx","Primary Iron 3.16E15 eV",100,0,1000);


//h2d= new TH2F("h2d","Primary proton 1E15 eV",1000,-500,500,1000,-500,500);


while (nlines<number_of_lines) {
	int ch;
	ch=fgetc(fp1);
	if (ch==EOF) break;

	ungetc(ch,fp1);
	fgets(linha,sizeof(linha),fp1);

     (void)sscanf(linha,"%f" "%f" "%f" "%f" "%f" , &eas, &part, &x_p, &y_p ,&r_p);
     	 if (r_p <= 1000){
         //cout << r_p << endl;
	         w = 1/(316*2*TMath::Pi()*r_p*d_r_p);
         //cout << TMath::Pi() << " " << r_p << " " << part << " "<< d_r_p << " " << w << endl;
    		 hpx->Fill(r_p,w);
    	}
		//r1=sqrt((x_p-xdet1)*(x_p-xdet1)+(y_p-ydet1)*(y_p-ydet1));
		//r2=sqrt((x_p-xdet2)*(x_p-xdet2)+(y_p-ydet2)*(y_p-ydet2));
		//r3=sqrt((x_p-xdet3)*(x_p-xdet3)+(y_p-ydet3)*(y_p-ydet3));

//		if ((r_p>=281)&&(r_p<=411))
/*		if ((r1<=rdet)||(r2<=rdet)||(r3<=rdet))
			{
			//cout << r_p << endl;
			h2d->Fill(x_p,y_p,0.01);
			npart++;
			}
    
*/	nlines++;
   }
c1->Update();
hpx->GetYaxis()->SetTitle("#rho (particles cm^{-2}) ");
hpx->GetXaxis()->SetTitle("r (cm)");
//c1->SetLogx();
hpx->Draw();

Double_t integral = 0.0;
Double_t delta_r = hpx->GetBinWidth(1);

for (Int_t i = 1; i <= hpx->GetNbinsX(); i++) {
	    Double_t r = hpx->GetBinCenter(i);
	        Double_t rho = hpx->GetBinContent(i); // Densidade no bin i
		    Double_t area = 2 *316*TMath::Pi()*r*delta_r; // Área do anel
		        integral += rho*area;
}

std::cout << "Número médio de partículas: " << integral << std::endl;


// Função com C, a, b e d
//    TF1 *f = new TF1("fit", "([0]/([3]*[3]))*exp(-[1]*log(x/[3]))*exp(-[2]*log(1+(x/[3])))*(1+0.35*exp([4]*log(x/[3])))", 0, 1000000);
//    f->SetParameters(50000, 1, 2, 7800, 0.5);
//    f->SetParLimits(1, 1.2, 2.2);
//    f->SetParLimits(2, 1.2, 5);
//    f->FixParameter(3, 7800);
//    f->SetParNames("C", "a", "b", "r0", "d");
//    hpx->Fit("fit");


// Função com C, a e b
//TF1 *f = new TF1("fit", "([0]/([3]*[3]))*TMath::Exp(-[1]*TMath::Log(x/[3]))*TMath::Exp(-[2]*TMath::Log(1+(x/[3])))",0,1000);
	
//	f->SetParameters(50000,1,3,7800);
	//f->FixParameter(0,0.0006);
	//f->FixParameter(1,0.88);
//	f->SetParLimits(1,1.2,2.2);
//	f->SetParLimits(2,1.2,6);
//	f->FixParameter(3,7800);
//	f->SetParNames("C","a","b","r0");
//	hpx->Fit("fit");

// Função com C e s
//TF1 *f = new TF1("fit", "([0]/([2]*[2]))*TMath::Exp(([1]-2)*TMath::Log(x/[2]))*TMath::Exp(([1]-4.5)*TMath::Log(1+(x/[2])))",0,1000);	
//
//	f->SetParameters(300,1,7800);
//	//f->FixParameter(0,0.0006);
//	//f->FixParameter(1,0.88);
//	f->SetParLimits(1,1.2,2.2);
//	f->FixParameter(2,7800);
//	f->SetParNames("C","s","r0");
//	hpx->Fit("fit");

// Função com C e s
TF1 *f = new TF1("fit", "([0]/([2]*[2]))*TMath::Exp(([1])*TMath::Log(x/[2]))",0,1000);	

	f->SetParameters(1000,-0.5,7800);
	//f->FixParameter(0,0.0006);
	//f->FixParameter(1,0.88);
	f->SetParLimits(1,-1,2);
        f->FixParameter(2,7800);
	f->SetParNames("C","s","r0");
	hpx->Fit("fit");

// Creates a Root function
/* TF1 *f = new TF1("fit","[0]*TMath::Exp(-[1]*TMath::Log(x))",1,300);
	
 f->SetParameters(20000,2.7);
 f->SetParNames("Normal.","Inclinacao");

 hpx->Fit("fit");*/


//  c2->Update();
//  h2d->Draw();
c1->SaveAs("iron3.16E15_3det.pdf");
}




