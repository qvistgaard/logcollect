CPP_LIBS = -L/usr/local/lib -lclucene-core -lpcre
CPP_INCLUDE = -Isrc/ -I/usr/local/include
CPP_FLAGS = -Wall -O3 -ansi

LIBS_INCLUDE = -I/usr/local/include

LOGCOLLECTD_LIBS = ${CPP_LIBS}
LOGCOLLECTD_INCLUDE= ${CPP_INCLUDE}

SEARCH_LIBS = ${CPP_LIBS} -ljansson
SEARCH_INCLUDE= ${CPP_INCLUDE}

All:
	g++ -c -o lib/Logger.o             lib/Logger.cpp             ${LIBS_INCLUDE} ${CPP_FLAGS} -pedantic
	g++ -c -o lib/Pattern.o            lib/Pattern.cpp            ${LIBS_INCLUDE} ${CPP_FLAGS}
	g++ -c -o lib/PatternFile.o        lib/PatternFile.cpp        ${LIBS_INCLUDE} ${CPP_FLAGS} -pedantic
	g++ -c -o lib/Patterns.o           lib/Patterns.cpp           ${LIBS_INCLUDE} ${CPP_FLAGS} -pedantic
	g++ -c -o lib/Result.o             lib/Result.cpp             ${LIBS_INCLUDE} ${CPP_FLAGS} -pedantic
	g++ -c -o lib/CluceneIndex.o       lib/CluceneIndex.cpp       ${LIBS_INCLUDE} ${CPP_FLAGS} -pedantic
	g++ -c -o lib/CluceneIndexReader.o lib/CluceneIndexReader.cpp ${LIBS_INCLUDE} ${CPP_FLAGS}
	g++ -c -o lib/DateConversion.o     lib/DateConversion.cpp     ${LIBS_INCLUDE} ${CPP_FLAGS}
	
	g++ -o src/logcollectd \
		src/logcollectd.cpp lib/Logger.o lib/Pattern.o lib/Patterns.o \
		lib/PatternFile.o lib/Result.o lib/CluceneIndex.o lib/DateConversion.o \
		${LOGCOLLECTD_LIBS} ${LOGCOLLECTD_INCLUDE} ${CPP_FLAGS}
		
	g++ -o src/search \
	       src/search.cpp lib/CluceneIndexReader.o \
		   ${SEARCH_LIBS} ${SEARCH_INCLUDE} ${CPP_FLAGS} 
	
	g++ -o src/test \
	       src/test.cpp \
		   ${SEARCH_LIBS} ${SEARCH_INCLUDE} ${CPP_FLAGS}
clean:
	rm lib/Logger.o lib/Pattern.o lib/PatternFile.o lib/Patterns.o \
	   lib/Result.o lib/CluceneIndex.o src/logcollectd src/search \
	   lib/CluceneIndexReader.o

