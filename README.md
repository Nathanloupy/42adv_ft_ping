# ft_ping

This project is about recoding the ping command, taking the inetutils-2.0 as a reference.

### Usage

```bash
$> make
$> ./ft_ping --help
Usage: ft_ping [OPTIONS] <destination>

Send ICMP ECHO_REQUEST packets to network hosts.

Options:
  -c, --count NUMBER  stop after sending NUMBER packets
  -q, --quiet         quiet output
  -s, --size NUMBER   send NUMBER data octets
  --ttl N             specify N as time-to-live
  -v, --verbose       verbose output
  -w, --timeout N     stop after N seconds
  --help              give this help list
  --usage             give a short usage message
```

### Examples

```bash
# Basic ping
$> ./ft_ping google.com

# Ping with specific count
$> ./ft_ping -c 4 google.com

# Verbose output with custom packet size
$> ./ft_ping -v -s 100 8.8.8.8

# Quiet mode with timeout
$> ./ft_ping -q -w 10 localhost
```

## Grading

This project is part of my curriculum at 42 School.

- **Date of completion:** 2025-07-30
- **Grade:** 125/100
