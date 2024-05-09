{
   gROOT->Reset();

//gStyle->SetOptLogx();
gStyle->SetOptLogy();
gStyle->SetOptFit(1011);
Char_t linha[10000];

    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile("/home/orfei/mestrado/progs/3_detectores/bary_dat/proton1E15_bary.csv");

    while (std::getline(myfile, line))
        ++number_of_lines;
    std::cout << "Numero de linhas: " << number_of_lines << "\n";

FILE *fp1 = fopen("/home/orfei/mestrado/progs/3_detectores/bary_dat/proton1E15_bary.csv","r");

Int_t nlines=0,npart=0;
Float_t rho, r_b;

Float_t xdet1=0,ydet1=346;
Float_t xdet2=-400,ydet2=-346;
Float_t xdet3=400,ydet3=-346;

Float_t r1_det,r2_det,r3_det,rdet=65;

r1_det=sqrt((xdet1)*(xdet1)+(ydet1)*(ydet1));
r2_det=sqrt((xdet2)*(xdet2)+(ydet2)*(ydet2));
r3_det=sqrt((xdet3)*(xdet3)+(ydet3)*(ydet3));

sqrt_lin = TMath::Sqrt(number_of_lines);



c1 = new TCanvas("c1","Histograma unidimensional",10,10,700,700);

//c1->SetLogx(1);
gr = new TGraph();
gr->SetTitle("Graph title;X-Axis;Y-Axis");

//while (nlines<number_of_lines/100) {
while (nlines<number_of_lines) {
	int ch;
	ch=fgetc(fp1);
	if (ch==EOF) break;

	ungetc(ch,fp1);
	fgets(linha,sizeof(linha),fp1);

     (void)sscanf(linha,"%f" "%f" , &rho, &r_b);
         cout << rho << " " <<  r_b << endl;
         gr->AddPoint(r_b, rho);                  
         
         nlines++;
   }
c1->Update();

gr->SetMarkerColor(4);
gr->SetMarkerStyle(20);
gr->Draw("AP");
gr->GetXaxis()->CenterTitle();


// Lateral distribution function - Nishimura-Kamata-Greisen (NKG)

TF1 *f = new TF1("fit", "([0]/([3]*[3]))*TMath::Exp(-[1]*TMath::Log(x/[3]))*TMath::Exp(-[2]*TMath::Log(1+(x/[3])))",0,1000000);
	
	f->SetParameters(300,1,0.5,1);
	//f->FixParameter(0,0.0006);
	//f->FixParameter(1,0.88);
	//f->FixParameter(2,1);
	f->SetParNames("C","a","b","r0");
	gr->Fit("fit");
}




