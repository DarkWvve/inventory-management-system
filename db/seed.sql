insert or ignore into roles(id,name) values
                  (1, 'admin'),
                  (2, 'manager'),
                  (3, 'worker');

insert or ignore into categories(id,name) values
                  (1, 'default');

insert or ignore into users(id,login,
                            password_hash,
                            full_name,
                            role_id,
                            is_active) values
                            (0,'test','test_hash','test_user',3,1),
                            (1,'admin','admin','admin_user',1,1),
                            (2,'manager','manager','manager_user',2,1);
