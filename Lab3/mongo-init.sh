set -e

mongo <<EOF
db = db.getSiblingDB('deliverydb')
db.createCollection('delivery')
db.delivery.createIndex({id: -1})
db.delivery.createIndex({sender_id: -1})
db.delivery.createIndex({receiver_id: -1})
db.delivery.createIndex({date: new Date("1970-01-01T00:00:00Z")})
db.delivery.createIndex({package: { weight: 0.0, height: 0.0, width: 0.0, length: 0.0, description: "" }})
EOF