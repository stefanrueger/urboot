FROM --platform=linux/amd64 ubuntu:jammy

RUN apt-get update \
	&& apt-get install -y build-essential perl \
	&& rm -rf /var/lib/apt/lists/*

WORKDIR /src

RUN PERL_MM_USE_DEFAULT=1 cpan -T install File::Basename Capture::Tiny POSIX Scalar::Util Getopt::Long

ENTRYPOINT ["/usr/bin/make"]
