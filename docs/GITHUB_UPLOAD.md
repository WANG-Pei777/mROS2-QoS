# GitHub Upload Checklist

This repository contains a real-hardware mROS2-ESP32 QoS validation project.

## Do Not Commit Local Files

The following files are intentionally local-only:

```text
platform/wifi/wifi_secrets.h
platform/rtps/config_local.h
build/
results/
workspace/*/build/
```

Use `git status --ignored --short` if you need to confirm they are ignored.

## Repository Shape

This upload package is intentionally flattened into one repository.
`mros2/` and `mros2/embeddedRTPS/` are normal source directories here, not git submodules.

That makes the repository easier to open and inspect on GitHub.
No `git submodule update` step is required after cloning this upload repository.

Commit and push from the repository root:

```bash
git status
git add .
git commit -m "Add ESP32 QoS hardware validation"
git push -u origin main
```

## Quick Public-Safety Check

Before pushing, run:

```bash
git status --short --ignored
grep -RInE "<local-ip-fragment>|<local-user-home>|<known-password-fragment>" \
  --exclude-dir=.git --exclude-dir=build --exclude-dir=results .
```

The real WiFi password and local WSL IP must only appear in ignored local files.
