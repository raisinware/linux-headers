/*
 * Constants and structure definitions for the bridging code
 */

#if !defined(One)
#define Zero    0
#define One	1
#endif  /* !defined(One) */

#if !defined(TRUE)
#define FALSE   0
#define TRUE	1
#endif /* !defined(TRUE) */

/** port states. **/
#define Disabled	0			  /* (4.4 5)	 */
#define Listening	1			  /* (4.4.2)	 */
#define Learning	2			  /* (4.4.3)	 */
#define Forwarding	3			  /* (4 4 4)	 */
#define Blocking	4			  /* (4.4.1)	 */

#define No_of_ports 8
/* arbitrary choice, to allow the code below to compile */

#define All_ports (No_of_ports + 1)

/*
 * We time out our entries in the FDB after this many seconds.
 */
#define FDB_TIMEOUT	20 /* JRP: 20s as NSC bridge code, was 300 for Linux */

/*
 * the following defines are the initial values used when the 
 * bridge is booted.  These may be overridden when this bridge is
 * not the root bridge.  These are the recommended default values 
 * from the 802.1d specification.
 */
#define BRIDGE_MAX_AGE		20
#define BRIDGE_HELLO_TIME	2
#define BRIDGE_FORWARD_DELAY	15
#define HOLD_TIME		1

/* broacast/multicast storm limitation. This per source. */
#define MAX_MCAST_PER_PERIOD    4
#define MCAST_HOLD_TIME		(10*HZ/100)

#define Default_path_cost 10

/*
 * minimum increment possible to avoid underestimating age, allows for BPDU
 * transmission time
 */
#define Message_age_increment 1

#define No_port 0
/*
 * reserved value for Bridge's root port parameter indicating no root port,
 * used when Bridge is the root - also used to indicate the source when
 * a frame is being generated by a higher layer protocol on this host
 */

/** Configuration BPDU Parameters (4.5.1) **/

typedef struct {
	union {
		struct {
			unsigned short priority;
			unsigned char ula[6];
		} p_u;
		unsigned int id[2];
	} bi;
} bridge_id_t;

#define BRIDGE_PRIORITY	bi.p_u.priority
#define BRIDGE_ID_ULA	bi.p_u.ula
#define BRIDGE_ID	bi.id

/* JRP: on the network the flags field is between "type" and "root_id"
 * this is unfortunated! To make the code portable to a RISC machine
 * the pdus are now massaged a little bit for processing
 */ 
#define TOPOLOGY_CHANGE		0x01
#define TOPOLOGY_CHANGE_ACK	0x80
#define BRIDGE_BPDU_8021_CONFIG_SIZE            35	/* real size */
#define BRIDGE_BPDU_8021_CONFIG_FLAG_OFFSET	 4
#define BRIDGE_BPDU_8021_PROTOCOL_ID 0
#define BRIDGE_BPDU_8021_PROTOCOL_VERSION_ID 0
#define BRIDGE_LLC1_HS 3
#define BRIDGE_LLC1_DSAP 0x42
#define BRIDGE_LLC1_SSAP 0x42
#define BRIDGE_LLC1_CTRL 0x03

typedef struct {
	unsigned short	protocol_id;	
	unsigned char	protocol_version_id;
	unsigned char   type;
	bridge_id_t      root_id;		  /* (4.5.1.1)	 */
	unsigned int     root_path_cost;	  /* (4.5.1.2)	 */
	bridge_id_t      bridge_id;		  /* (4.5.1.3)	 */
	unsigned short   port_id;		  /* (4.5.1.4)	 */
	unsigned short   message_age;		  /* (4.5.1.5)	 */
	unsigned short   max_age;		  /* (4.5.1.6)	 */
	unsigned short   hello_time;		  /* (4.5.1.7)	 */
	unsigned short   forward_delay;		  /* (4.5.1.8)	 */
	unsigned char   top_change_ack;
	unsigned char   top_change;
} Config_bpdu;

#ifdef __LITTLE_ENDIAN
#define config_bpdu_hton(config_bpdu) \
        (config_bpdu)->root_path_cost = htonl((config_bpdu)->root_path_cost); \
        (config_bpdu)->port_id = htons((config_bpdu)->port_id); \
        (config_bpdu)->message_age = htons((config_bpdu)->message_age); \
        (config_bpdu)->max_age = htons((config_bpdu)->max_age); \
        (config_bpdu)->hello_time = htons((config_bpdu)->hello_time); \
        (config_bpdu)->forward_delay = htons((config_bpdu)->forward_delay);
#else
#define config_bpdu_hton(config_bpdu)
#endif
#define config_bpdu_ntoh config_bpdu_hton


/** Topology Change Notification BPDU Parameters (4.5.2) **/

typedef struct {
	unsigned short	protocol_id;	
	unsigned char	protocol_version_id;
	unsigned char   type;
} Tcn_bpdu;

#define BPDU_TYPE_CONFIG	0
#define BPDU_TYPE_TOPO_CHANGE	128

/** Bridge Parameters (4.5.3) **/
typedef struct {
	bridge_id_t      designated_root;	  /* (4.5.3.1)	 */
	unsigned int     root_path_cost;	  /* (4.5.3.2)	 */
	unsigned int 	 root_port;		  /* (4.5.3.3)	 */
	unsigned short   max_age;		  /* (4.5.3.4)	 */
	unsigned short   hello_time;		  /* (4.5.3.5)	 */
	unsigned short   forward_delay;		  /* (4.5.3.6)	 */
	bridge_id_t      bridge_id;		  /* (4.5.3.7)	 */
	unsigned short   bridge_max_age;	  /* (4.5.3.8)	 */
	unsigned short   bridge_hello_time;	  /* (4.5.3.9)	 */
	unsigned short   bridge_forward_delay;	  /* (4.5.3.10)	 */
	unsigned int     top_change_detected;	  /* (4.5.3.11) */
	unsigned int     top_change;		  /* (4.5.3.12)	 */
	unsigned short   topology_change_time;	  /* (4.5.3.13)	 */
	unsigned short   hold_time;		  /* (4.5.3.14)	 */
} Bridge_data;

/** Port Parameters (4.5.5) **/
typedef struct {
	unsigned short   port_id;		  /* (4.5.5.1)	 */
	unsigned int     state;			  /* (4.5.5.2)	 */
	unsigned int     path_cost;		  /* (4.5.5.3)	 */
	bridge_id_t      designated_root;	  /* (4.5.5.4)	 */
	unsigned int     designated_cost;	  /* (4.5.5.5)	 */
	bridge_id_t      designated_bridge;	  /* (4.5.5.6)	 */
	unsigned short   designated_port;	  /* (4.5.5.7)	 */
	unsigned int     top_change_ack;	  /* (4.5.5.8)	 */
	unsigned int     config_pending;	  /* (4.5.5.9)	 */
	struct device *dev;	
	struct fdb *fdb;	/* head of per port fdb chain */
} Port_data;



/** types to support timers for this pseudo-implementation. **/
typedef struct {
	unsigned int     active;			  /* timer in use. */
	unsigned int     value;			  /* current value of timer,
						   * counting up. */
} Timer;

struct fdb {
	unsigned char ula[6];
	unsigned char pad[2];
	unsigned short port;
	unsigned int timer;
	unsigned short flags;
#define FDB_ENT_VALID	0x01
	unsigned short mcast_count;
	unsigned int   mcast_timer;		/* oldest xxxxxcast */
	
/* AVL tree of all addresses, sorted by address */
	short fdb_avl_height;
	struct fdb *fdb_avl_left;
	struct fdb *fdb_avl_right;
/* linked list of addresses for each port */
	struct fdb *fdb_next;
};

/* data returned on BRCMD_DISPLAY_FDB */
struct fdb_info {
	unsigned char ula[6];
	unsigned char port;
        unsigned char flags;
        unsigned int timer;
};
struct fdb_info_hdr {
	int	copied;			/* nb of entries copied to user */
	int	not_copied;		/* when user buffer is too small */
	int	cmd_time;
};	

#define IS_BRIDGED	0x2e


#define BR_MAX_PROTOCOLS 32
#define BR_MAX_PROT_STATS BR_MAX_PROTOCOLS

/* policy values for policy field */
#define BR_ACCEPT 1
#define BR_REJECT 0

/* JRP: extra statistics for debug */
typedef struct {
	/* br_receive_frame counters */
	int port_disable_up_stack;
	int rcv_bpdu;
	int notForwarding;
	int forwarding_up_stack;
	int unknown_state;

	/* br_tx_frame counters */
	int port_disable;
	int port_not_disable;

	/* br_forward counters */
	int local_multicast;
	int forwarded_multicast;	/* up stack as well */
	int flood_unicast;
	int aged_flood_unicast;
	int forwarded_unicast;
	int forwarded_unicast_up_stack;
	int forwarded_ip_up_stack;
	int forwarded_ip_up_stack_lie;	/* received on alternate device */
	int arp_for_local_mac;
	int drop_same_port;
	int drop_same_port_aged;
	int drop_multicast;
} br_stats_counter;

struct br_stat {
	unsigned int flags;
	Bridge_data bridge_data;
	Port_data port_data[No_of_ports];
	unsigned int policy;
	unsigned int exempt_protocols;
	unsigned short protocols[BR_MAX_PROTOCOLS];
	unsigned short prot_id[BR_MAX_PROT_STATS];	/* Protocol encountered */
	unsigned int prot_counter[BR_MAX_PROT_STATS];	/* How many packets ? */
	br_stats_counter packet_cnts;
};

/* defined flags for br_stat.flags */
#define BR_UP		0x0001	/* bridging enabled */
#define BR_DEBUG	0x0002	/* debugging enabled */
#define BR_PROT_STATS	0x0004	/* protocol statistics enabled */
#define BR_STP_DISABLED	0x0008	/* Spanning tree protocol disabled */

struct br_cf {
	unsigned int cmd;
	unsigned int arg1;
	unsigned int arg2;
};

/* defined cmds */
#define	BRCMD_BRIDGE_ENABLE	1
#define	BRCMD_BRIDGE_DISABLE	2
#define	BRCMD_PORT_ENABLE	3	/* arg1 = port */
#define	BRCMD_PORT_DISABLE	4	/* arg1 = port */
#define	BRCMD_SET_BRIDGE_PRIORITY	5	/* arg1 = priority */
#define	BRCMD_SET_PORT_PRIORITY	6	/* arg1 = port, arg2 = priority */
#define	BRCMD_SET_PATH_COST	7	/* arg1 = port, arg2 = cost */
#define	BRCMD_DISPLAY_FDB	8	/* arg1 = port */
#define	BRCMD_ENABLE_DEBUG	9
#define	BRCMD_DISABLE_DEBUG	10
#define BRCMD_SET_POLICY	11	/* arg1 = default policy (1==bridge all) */
#define BRCMD_EXEMPT_PROTOCOL	12	/* arg1 = protocol (see net/if_ether.h) */
#define BRCMD_ENABLE_PROT_STATS	13
#define BRCMD_DISABLE_PROT_STATS 14
#define BRCMD_ZERO_PROT_STATS	15
#define BRCMD_TOGGLE_STP	16

/* prototypes of exported bridging functions... */

void br_init(void);
int br_receive_frame(struct sk_buff *skb);	/* 3.5 */
int br_tx_frame(struct sk_buff *skb);
int br_ioctl(unsigned int cmd, void *arg);
int br_protocol_ok(unsigned short protocol);
void requeue_fdb(struct fdb *node, int new_port);

struct fdb *br_avl_find_addr(unsigned char addr[6]);
struct fdb *br_avl_insert (struct fdb * new_node);
void sprintf_avl (char **pbuffer, struct fdb * tree, off_t *pos,int* len, off_t offset, int length);
int br_tree_get_info(char *buffer, char **start, off_t offset, int length, int dummy);
void br_avl_delete_by_port(int port);
/* externs */

extern struct br_stat br_stats;
extern Port_data port_info[];

