#include "tidydriver.h"
#include <QByteArray>
#include <tidy/tidy.h>
#include <tidy/tidybuffio.h>

TidyDriver::TidyDriver() {
}

QByteArray TidyDriver::cleanse1(const QByteArray& original) {
	TidyBuffer output = {0};
	TidyBuffer errbuf = {0};
	int        rc     = -1;
	bool       ok;

	TidyDoc tdoc = tidyCreate(); // Initialize "document"

	ok = tidyOptSetBool(tdoc, TidyXhtmlOut, yes); // Convert to XHTML
	ok &= tidyOptSetBool(tdoc, TidyDropEmptyElems, yes);
	if (ok)
		rc = tidySetErrorBuffer(tdoc, &errbuf); // Capture diagnostics
	if (rc >= 0)
		rc = tidyParseString(tdoc, original.constData()); // Parse the input
	if (rc >= 0)
		rc = tidyCleanAndRepair(tdoc); // Tidy it up!
	if (rc >= 0)
		rc = tidyRunDiagnostics(tdoc); // Kvetch
	if (rc > 1)                            // If error, force output.
		rc = (tidyOptSetBool(tdoc, TidyForceOutput, yes) ? rc : -1);
	if (rc >= 0)
		rc = tidySaveBuffer(tdoc, &output); // Pretty Print

	if (rc >= 0) {
		if (rc > 0) { //If you want to see how bad was
			      //printf("\nDiagnostics:\n\n%s", errbuf.bp);
		}
	} else {
		printf("A severe error (%d) occurred.\n", rc);
		return QByteArray();
	}
	QByteArray res;
	res.append((const char*)output.bp, output.size);

	tidyBufFree(&output);
	tidyBufFree(&errbuf);
	tidyRelease(tdoc);
	return res;
}
