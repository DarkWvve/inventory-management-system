CREATE TABLE roles (
                       id INTEGER PRIMARY KEY AUTOINCREMENT,
                       name TEXT NOT NULL UNIQUE
);

CREATE TABLE users (
                       id INTEGER PRIMARY KEY AUTOINCREMENT,
                       login TEXT NOT NULL UNIQUE,
                       password_hash TEXT NOT NULL,
                       full_name TEXT NOT NULL,
                       role_id INTEGER NOT NULL,
                       is_active INTEGER NOT NULL DEFAULT 1,

                       FOREIGN KEY (role_id) REFERENCES roles(id)
);

CREATE TABLE categories (
                            id INTEGER PRIMARY KEY AUTOINCREMENT,
                            name TEXT NOT NULL UNIQUE
);

CREATE TABLE suppliers (
                           id INTEGER PRIMARY KEY AUTOINCREMENT,
                           name TEXT NOT NULL,
                           phone TEXT,
                           email TEXT,
                           address TEXT
);

CREATE TABLE products (
                          id INTEGER PRIMARY KEY AUTOINCREMENT,
                          name TEXT NOT NULL,
                          sku TEXT NOT NULL UNIQUE,
                          category_id INTEGER NOT NULL,
                          unit TEXT NOT NULL,
                          min_quantity INTEGER NOT NULL DEFAULT 0,
                          is_active INTEGER NOT NULL DEFAULT 1,

                          FOREIGN KEY (category_id) REFERENCES categories(id),

                          CHECK (min_quantity >= 0)
);

CREATE TABLE stock_movements (
                                 id INTEGER PRIMARY KEY AUTOINCREMENT,
                                 product_id INTEGER NOT NULL,
                                 movement_type TEXT NOT NULL,
                                 quantity INTEGER NOT NULL,
                                 movement_date TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
                                 supplier_id INTEGER,
                                 user_id INTEGER NOT NULL,
                                 comment TEXT,

                                 FOREIGN KEY (product_id) REFERENCES products(id),
                                 FOREIGN KEY (supplier_id) REFERENCES suppliers(id),
                                 FOREIGN KEY (user_id) REFERENCES users(id),

                                 CHECK (movement_type IN ('IN', 'OUT')),
                                 CHECK (quantity > 0)
);

CREATE TABLE inventory_checks (
                                  id INTEGER PRIMARY KEY AUTOINCREMENT,
                                  check_date TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
                                  user_id INTEGER NOT NULL,
                                  status TEXT NOT NULL DEFAULT 'OPEN',
                                  comment TEXT,

                                  FOREIGN KEY (user_id) REFERENCES users(id),

                                  CHECK (status IN ('OPEN', 'CLOSED', 'CANCELLED'))
);

CREATE TABLE inventory_check_items (
                                       id INTEGER PRIMARY KEY AUTOINCREMENT,
                                       inventory_check_id INTEGER NOT NULL,
                                       product_id INTEGER NOT NULL,
                                       system_quantity INTEGER NOT NULL,
                                       actual_quantity INTEGER NOT NULL,
                                       difference INTEGER GENERATED ALWAYS AS (actual_quantity - system_quantity) STORED,

                                       FOREIGN KEY (inventory_check_id) REFERENCES inventory_checks(id) ON DELETE CASCADE,
                                       FOREIGN KEY (product_id) REFERENCES products(id),

                                       CHECK (system_quantity >= 0),
                                       CHECK (actual_quantity >= 0)
);

CREATE INDEX idx_users_role_id
    ON users(role_id);

CREATE INDEX idx_products_category_id
    ON products(category_id);

CREATE INDEX idx_stock_movements_product_id
    ON stock_movements(product_id);

CREATE INDEX idx_stock_movements_user_id
    ON stock_movements(user_id);

CREATE INDEX idx_stock_movements_supplier_id
    ON stock_movements(supplier_id);

CREATE INDEX idx_inventory_checks_user_id
    ON inventory_checks(user_id);

CREATE INDEX idx_inventory_check_items_check_id
    ON inventory_check_items(inventory_check_id);

CREATE INDEX idx_inventory_check_items_product_id
    ON inventory_check_items(product_id);