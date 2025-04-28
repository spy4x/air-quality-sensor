# syntax=docker/dockerfile:1

ARG NODE_VERSION=20.4.0
ARG PNPM_VERSION=8.6.7

FROM node:${NODE_VERSION}-alpine as base

WORKDIR /app

RUN npm install -g pnpm@${PNPM_VERSION}

COPY . .

RUN pnpm install

RUN pnpm build

USER node
ENV NODE_ENV production

CMD pnpm --filter ./packages/database migrate && node apps/web/build/index.js
