SELECT 
	users.alias AS user,
	sockets.alias AS socket,
	connections_audit.entry_type,
	connections_audit.entry_details,
	connections_audit.from,
	connections_audit.entry_time
FROM connections_audit
LEFT JOIN users ON users.id = connections_audit.user
LEFT JOIN sockets ON sockets.id = connections_audit.socket
ORDER BY connections_audit.entry_time DESC;