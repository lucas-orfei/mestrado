{
   gROOT->Reset();

//gStyle->SetOptLogx();
gStyle->SetOptLogy();
gStyle->SetOptFit(1011);
Char_t linha[10000];

    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile("/home/orfei/mestrado/progs/lateral_distribution/eletrons/data/iron/DAT4561514_tratado");

    while (std::getline(myfile, line))
        ++number_of_lines;
    std::cout << "Numero de linhas: " << number_of_lines << "\n";
    //return 0;


FILE *fp1 = fopen("/home/orfei/mestrado/progs/lateral_distribution/eletrons/data/iron/DAT4561514_tratado","r");

Int_t nlines=0,npart=0;
Float_t part,x_p, y_p, r_p,w,d_r_p;

Float_t xdet1=0,ydet1=346;
Float_t xdet2=-400,ydet2=-346;
Float_t xdet3=400,ydet3=-346;

Float_t r1,r2,r3,rdet=65;

//num_lin = 20976079;
sqrt_lin = TMath::Sqrt(number_of_lines);
d_r_p = 6.036;
r_0 = 78;


c1 = new TCanvas("c1","Histograma unidimensional",10,10,700,700);
//c2 = new TCanvas("c2","Histograma bidimensional",10,10,700,700);

//c1->SetLogx(1);

hpx= new TH1F("hpx","Primary iron 4.64E15 eV",165,0,1000);

//h2d= new TH2F("h2d","Primary proton 1E15 eV",1000,-500,500,1000,-500,500);


//while (nlines<number_of_lines/100) {
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
    
		r1=sqrt((x_p-xdet1)*(x_p-xdet1)+(y_p-ydet1)*(y_p-ydet1));
		r2=sqrt((x_p-xdet2)*(x_p-xdet2)+(y_p-ydet2)*(y_p-ydet2));
		r3=sqrt((x_p-xdet3)*(x_p-xdet3)+(y_p-ydet3)*(y_p-ydet3));

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
hpx->GetXaxis()->SetTitle("(cm)");
//c1->SetLogx();
hpx->Draw();

rho_sum = hpx->Integral(0,166);

cout << "Densidade de particulas: " << rho_sum << endl;

bin_529 = hpx->GetXaxis()->FindBin(529);
cout << "Numero do bin em 500 cm: " << bin_529 << endl;

rho_529 = hpx->Integral(0,88);

cout << "Número total de particulas até 529 cm: " << rho_529 << endl;

bin_346 = hpx->GetXaxis()->FindBin(346);
cout << "Numero do bin em 346 cm: " << bin_346 << endl;

rho_346 = hpx->Integral(58,58);

cout << "Densidade de particulas em 346 cm: " << rho_346 << endl;

//cout << "Numero medio de particulas em torno de 346 cm pelo h2: " << npart/300 << endl;

// Lateral distribution function - Nishimura-Kamata-Greisen (NKG)

//TF1 *f = new TF1("fit", "([0]/([3]*[3]))*TMath::Exp(-[1]*TMath::Log(x/[3]))*TMath::Exp(-[2]*TMath::Log(1+(x/[3])))",0,1000000);


TF1 *f = new TF1("fit", "([0]/([2]*[2]))*TMath::Exp(([1]-2)*TMath::Log(x/[2]))*TMath::Exp(([1]-4.5)*TMath::Log(1+(x/[2])))",0,1000000);	

	f->SetParameters(300,0.5,7800);
	//f->FixParameter(0,0.0006);
	//f->FixParameter(1,0.88);
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
}




