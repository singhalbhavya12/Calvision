#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <iostream>

TGraph* histToGraph(TH2F* hist, int color, int style, float size) {
    std::vector<double> x_vals, y_vals;
    for (int x = 1; x <= hist->GetNbinsX(); ++x) {
        for (int y = 1; y <= hist->GetNbinsY(); ++y) {
            if (hist->GetBinContent(x, y) > 0) {
                x_vals.push_back(hist->GetXaxis()->GetBinCenter(x));
                y_vals.push_back(hist->GetYaxis()->GetBinCenter(y));
            }
        }
    }

    TGraph* g = new TGraph(x_vals.size(), &x_vals[0], &y_vals[0]);
    g->SetMarkerColor(color);
    g->SetMarkerStyle(style);
    g->SetMarkerSize(size);
    g->SetLineColor(color);
    return g;
}


void plotOverlay()
{
TFile *f1 = TFile::Open("EPbWO4_2000/histoPbWO4/histo.root");
TFile *f2 = TFile::Open("BGlass_2000/histo.root");

TH2F *h1 = (TH2F*)f1->Get("hfncerEcal");
TH2F *h2 = (TH2F*)f1->Get("hfnscinEcal");

TGraph *g2 = histToGraph(h1, kGreen, 20, 0.5);   // PbWO4
TGraph *g1 = histToGraph(h2, kRed, 21, 0.6);  // Glass_ABS

TCanvas *c = new TCanvas("c", "Cherenkov vs Scintillation Signal for Calorimeter", 800, 600);
gStyle->SetOptStat(0);
// g1->GetXaxis()->SetRangeUser(0, 1.5);
// g1->GetYaxis()->SetRangeUser(0, 1.5);
g1->GetYaxis()->SetTitle("Signal");
g1->GetXaxis()->SetTitle("f");
g1->Draw("AP");        // A = axis, P = points
g2->Draw("P SAME");    // overlay points

TLegend *leg = new TLegend(0.15, 0.75, 0.4, 0.88);
leg->AddEntry(g1, "Cherenkov", "p");
leg->AddEntry(g2, "Scintillation", "p");

leg->Draw();

c->SaveAs("Scin_vs_cer_PbWO4.png");
}




//1D

// void plotOverlay()
// {
// gStyle->SetOptStat(0);
// TFile *f1 =TFile::Open("EPbWO4_2000/histoPbWO4/histo.root");
// TFile *f2 =TFile::Open("BGlass_2000/histo.root");
// // TH1F *h1 = (TH1F*)f1->Get("hfncerEcal");
// TH1F *h1 = (TH1F*)f1->Get("hfncerEcal");
// TH1F *h2 = (TH1F*)f1->Get("hfnscinEcal");
// TH1F *h3 = (TH1F*)f1->Get("phcEcalcorr");
// h1->Scale(1.0 / h1->Integral());
// h2->Scale(1.0 / h2->Integral());
// h3->Scale(1.0 / h3->Integral());
// h1->SetLineColor(kGreen);
// h1->SetLineStyle(2);
// h2->SetLineStyle(1);
// // h1->SetMarkerColor(kBlue);
// // h1->SetMarkerStyle(24);
// // h1->SetMarkerSize(0.5);

// h2->SetLineColor(kRed);
// // h2->SetMarkerColor(kBlue);
// // h2->SetMarkerStyle(25);
// // h2->SetMarkerSize(0.5);

// h3->SetLineColor(kGreen);

// h1->GetXaxis()->SetRangeUser(0, 1.4);
// h1->GetYaxis()->SetRangeUser(0,1.4);
// h1->GetYaxis()->SetTitle("Percent");
// h1->GetXaxis()->SetTitle("Signal(Callibrated to electrons)");

// TLegend *leg = new TLegend(0.65, 0.75, 0.4, 0.88);
// leg->AddEntry(h1, "Cherenkov", "l");
// leg->AddEntry(h2, "Scintillation", "l");
// // leg->AddEntry(h3, "DR correction", "l");
// TCanvas *c1 = new TCanvas("c1", "energy distributions for calorimeter", 800, 600);
// c1->SetMargin(0.13, 0.05, 0.13, 0.05);
// h1->Rebin(4);
// h2->Rebin(4);
// // h3->Rebin(4);
// h1->Draw("hist");
// h2->Draw("hist Same");
// // h3->Draw("hist Same");
// leg->Draw();

// c1->SaveAs("Scin_vs_cer_PbWO4.png");
// }