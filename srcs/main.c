#include "commons.h"

int main(int argc, char *argv[])
{
	int socket_fd;
	int ret;
	struct protoent *proto;
	struct sockaddr_in dest_addr;
	struct icmp icmp_hdr;
	char packet[64];
	struct timeval start, end;
	double elapsed_time;

	if (argc != 2)
	{
		printf("Usage: %s <destination_ip>\n", argv[0]);
		return (1);
	}

	proto = getprotobyname("icmp");
	if (!proto)
		return (1);
	socket_fd = socket(PF_INET, SOCK_RAW, proto->p_proto);
	if (socket_fd == -1)
	{
		perror("socket");
		return (1);
	}

	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;
	if (inet_pton(AF_INET, argv[1], &dest_addr.sin_addr) <= 0)
	{
		perror("inet_pton");
		close(socket_fd);
		return (1);
	}

	memset(&icmp_hdr, 0, sizeof(icmp_hdr));
	icmp_hdr.icmp_type = ICMP_ECHO;
	icmp_hdr.icmp_code = 0;
	icmp_hdr.icmp_seq = 1;
	icmp_hdr.icmp_id = getpid() & 0xFFFF;

	memset(packet, 0, sizeof(packet));
	memcpy(packet, &icmp_hdr, sizeof(icmp_hdr));
	icmp_hdr.icmp_cksum = checksum(packet, sizeof(icmp_hdr));
	memcpy(packet, &icmp_hdr, sizeof(icmp_hdr));

	gettimestamp(&start);

	ret = sendto(socket_fd, packet, sizeof(icmp_hdr), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
	if (ret <= 0)
	{
		perror("sendto");
		close(socket_fd);
		return (1);
	}

	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
	{
		perror("setsockopt");
		close(socket_fd);
		return (1);
	}

	char recv_buf[512];
	struct sockaddr_in from_addr;
	socklen_t from_len = sizeof(from_addr);
	
	ret = recvfrom(socket_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr*)&from_addr, &from_len);
	if (ret <= 0)
	{
		perror("recvfrom");
		printf("No reply received (timeout)\n");
	}
	else
	{
		gettimestamp(&end);
		struct ip* ip_hdr = (struct ip*)recv_buf;
		int ip_header_len = ip_hdr->ip_hl * 4;
		struct icmp* icmp_reply = (struct icmp*)(recv_buf + ip_header_len);

		if (icmp_reply->icmp_type == ICMP_ECHOREPLY && icmp_reply->icmp_id == (getpid() & 0xFFFF))
		{
			char from_ip[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &(from_addr.sin_addr), from_ip, INET_ADDRSTRLEN);

			elapsed_time = getelapsedtime_ms(&start, &end);

			printf("Reply from %s: icmp_seq=%d ttl=%d time=%.2f ms\n", from_ip, icmp_reply->icmp_seq, ip_hdr->ip_ttl, elapsed_time);
		}
		else
		{
			printf("Received packet but not our ECHO REPLY\n");
		}
	}
	close(socket_fd);
	return (0);
}
