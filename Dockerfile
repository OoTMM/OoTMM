FROM debian:bookworm

RUN apt-get update \
  && DEBIAN_FRONTEND=noninteractive apt-get install --no-install-recommends --assume-yes curl gnupg ca-certificates \
  && curl https://practicerom.com/public/packages/debian/pgp.pub | apt-key add - \
  && echo deb http://practicerom.com/public/packages/debian staging main >/etc/apt/sources.list.d/practicerom.list \
  && apt-get update \
  && DEBIAN_FRONTEND=noninteractive apt-get install --no-install-recommends --assume-yes \
    build-essential \
    n64-ultra \
    ruby \
    ruby-dev \
    git \
  && gem install bundler \
  && useradd -ms /bin/bash ootmm

WORKDIR /app
ENTRYPOINT [ "/app/docker-entrypoint.sh" ]
CMD [ "/app/bin/combo" ]
