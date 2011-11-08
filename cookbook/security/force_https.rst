異なる URL で HTTPS や HTTP を強制化させる方法
=============================================

セキュリティの設定で、サイトの一部に ``HTTPS`` プロトコルの使用を強制化させることができます。方法は、 ``access_control`` ルールに ``requires_channel`` のオプションを使用することで行います。例えば、 ``/secure`` から始まる全ての URL に ``HTTPS`` を強制化させるには、次のコンフィギュレーションを使用してください。

.. configuration-block::

        .. code-block:: yaml

            access_control:
                - path: ^/secure
                  roles: ROLE_ADMIN
                  requires_channel: https

        .. code-block:: xml

            <access-control>
                <rule path="^/secure" role="ROLE_ADMIN" requires_channel="https" />
            </access-control>

        .. code-block:: php

            'access_control' => array(
                array('path' => '^/secure', 
                      'role' => 'ROLE_ADMIN', 
                      'requires_channel' => 'https'
                ),
            ),

ログインフォーム自体は、ユーザを認証させる必要があるので、匿名アクセスが可能できなければなりません。 ``HTTPS`` を使用してそれを実現するには、 ``access_control`` ルールに ``IS_AUTHENTICATED_ANONYMOUSLY`` 権限を使用します。

.. configuration-block::

        .. code-block:: yaml

            access_control:
                - path: ^/login
                  roles: IS_AUTHENTICATED_ANONYMOUSLY
                  requires_channel: https

        .. code-block:: xml

            <access-control>
                <rule path="^/login" 
                      role="IS_AUTHENTICATED_ANONYMOUSLY" 
                      requires_channel="https" />
            </access-control>

        .. code-block:: php

            'access_control' => array(
                array('path' => '^/login', 
                      'role' => 'IS_AUTHENTICATED_ANONYMOUSLY', 
                      'requires_channel' => 'https'
                ),
            ),

ルーティングコンフィギュレーションに ``HTTPS`` の使用を指定することも可能です。詳細は、 :doc:`/cookbook/routing/scheme` を参照してください。

.. 2011/11/08 ganchiku c4b4bfa0b6ad56253f69706e4b461e4f79520302

