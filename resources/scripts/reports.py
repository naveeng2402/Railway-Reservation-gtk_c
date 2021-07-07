from weasyprint import HTML
pdf = HTML("rsc/report.html").write_pdf()
open("rsc/report.pdf", "wb").write(pdf)
png = HTML("rsc/report.html").write_png()
open("rsc/report.png", "wb").write(png)