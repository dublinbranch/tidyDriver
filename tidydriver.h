#ifndef TIDYDRIVER_H
#define TIDYDRIVER_H

class QByteArray;
class TidyDriver {
      public:
	TidyDriver();
	QByteArray cleanse1(const QByteArray& original);
};

#endif // TIDYDRIVER_H
