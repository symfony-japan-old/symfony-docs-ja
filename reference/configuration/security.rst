.. index::
   single: Security; Configuration reference

.. note::

    * 対象バージョン：2.4
    * 翻訳更新日：2013/12/06

SecurityBundle 設定 ("security")
================================

Symfony2のとくに強力な要素のひとつであるセキュリティシステムは、この設
定からほとんどコントロールすることができます。

すべての設定値の説明および初期値
--------------------------------

セキュリティシステムのすべての設定の初期値は次のとおりです。それぞれの
項目については次のセクションで説明していきます。

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            access_denied_url:    ~ # 例: /foo/error403

            # strategyに設定できる値は: none, migrate, invalidate
            session_fixation_strategy:  migrate
            hide_user_not_found:  true
            always_authenticate_before_granting:  false
            erase_credentials:    true
            access_decision_manager:
                strategy:             affirmative
                allow_if_all_abstain:  false
                allow_if_equal_granted_denied:  true
            acl:

                # いずれも doctrine.dbal セクションで設定されます
                connection:           ~
                cache:
                    id:                   ~
                    prefix:               sf2_acl_
                provider:             ~
                tables:
                    class:                acl_classes
                    entry:                acl_entries
                    object_identity:      acl_object_identities
                    object_identity_ancestors:  acl_object_identity_ancestors
                    security_identity:    acl_security_identities
                voter:
                    allow_if_object_identity_unavailable:  true

            encoders:
                # 例:
                Acme\DemoBundle\Entity\User1: sha512
                Acme\DemoBundle\Entity\User2:
                    algorithm:           sha512
                    encode_as_base64:    true
                    iterations:          5000

                # PBKDF2 エンコーダ
                # 詳しくは下のPBKDF2に関するノート "セキュリティとスピード" を参照してください
                Acme\Your\Class\Name:
                    algorithm:            pbkdf2
                    hash_algorithm:       sha512
                    encode_as_base64:     true
                    iterations:           1000

                # カスタムエンコーダの options/values の例
                Acme\DemoBundle\Entity\User3:
                    id:                   my.encoder.id

            providers:            # 必須
                # 例:
                my_in_memory_provider:
                    memory:
                        users:
                            foo:
                                password:           foo
                                roles:              ROLE_USER
                            bar:
                                password:           bar
                                roles:              [ROLE_USER, ROLE_ADMIN]

                my_entity_provider:
                    entity:
                        class:              SecurityBundle:User
                        property:           username
                        manager_name:       ~

                # カスタムプロバイダの例
                my_some_custom_provider:
                    id:                   ~

                # 連なったプロバイダ
                my_chain_provider:
                    chain:
                        providers:          [ my_in_memory_provider, my_entity_provider ]

            firewalls:            # 必須
                # 例:
                somename:
                    pattern: .*
                    request_matcher: some.service.id
                    access_denied_url: /foo/error403
                    access_denied_handler: some.service.id
                    entry_point: some.service.id
                    provider: some_key_from_above
                    # それぞれのファイアウォールが蓄えるセッション情報を管理します
                    # 詳しくは下の "ファイアウォールのコンテキスト" を参照してください
                    context: context_key
                    stateless: false
                    x509:
                        provider: some_key_from_above
                    http_basic:
                        provider: some_key_from_above
                    http_digest:
                        provider: some_key_from_above
                    form_login:
                        # ここで、ログインフォームを送信します
                        check_path: /login_check

                        # ログインが必要なとき、ユーザはここにリダイレクトされます
                        login_path: /login

                        # trueのとき、リダイレクトの代わりにユーザをログインフォームにフォワードします
                        use_forward: false

                        # ログイン成功時のリダイレクトオプション（下の説明を読んでください）
                        always_use_default_target_path: false
                        default_target_path:            /
                        target_path_parameter:          _target_path
                        use_referer:                    false

                        # ログイン失敗時のリダイレクトオプション（下の説明を読んでください）
                        failure_path:    /foo
                        failure_forward: false
                        failure_path_parameter: _failure_path
                        failure_handler: some.service.id
                        success_handler: some.service.id

                        # ユーザ名とパスワードのフィールド名
                        username_parameter: _username
                        password_parameter: _password

                        # csrf トークンのオプション
                        csrf_parameter: _csrf_token
                        intention:      authenticate
                        csrf_provider:  my.csrf_provider.id

                        # 初期値では、ログインフォームはGETではなく、POSTメソッドで*なければなりません*
                        post_only:      true
                        remember_me:    false

                        # 初期値では、認証リクエストが送信される前にセッションが存在している必要があります
                        # falseにすると認証の間にRequest::hasPreviousSessionが呼び出されません
                        # Symfony 2.3の新機能です
                        require_previous_session: true

                    remember_me:
                        token_provider: name
                        key: someS3cretKey
                        name: NameOfTheCookie
                        lifetime: 3600 # 秒単位
                        path: /foo
                        domain: somedomain.foo
                        secure: false
                        httponly: true
                        always_remember_me: false
                        remember_me_parameter: _remember_me
                    logout:
                        path:   /logout
                        target: /
                        invalidate_session: false
                        delete_cookies:
                            a: { path: null, domain: null }
                            b: { path: null, domain: null }
                        handlers: [some.service.id, another.service.id]
                        success_handler: some.service.id
                    anonymous: ~

                # ファイアウォールの初期値とオプション
                some_firewall_listener:
                    pattern:              ~
                    security:             true
                    request_matcher:      ~
                    access_denied_url:    ~
                    access_denied_handler:  ~
                    entry_point:          ~
                    provider:             ~
                    stateless:            false
                    context:              ~
                    logout:
                        csrf_parameter:       _csrf_token
                        csrf_provider:        ~
                        intention:            logout
                        path:                 /logout
                        target:               /
                        success_handler:      ~
                        invalidate_session:   true
                        delete_cookies:

                            # プロトタイプ
                            name:
                                path:                 ~
                                domain:               ~
                        handlers:             []
                    anonymous:
                        key:                  4f954a0667e01
                    switch_user:
                        provider:             ~
                        parameter:            _switch_user
                        role:                 ROLE_ALLOWED_TO_SWITCH

            access_control:
                requires_channel:     ~

                # URLデコードされた形式をつかいます
                path:                 ~ # 例: ^/path to resource/
                host:                 ~
                ip:                   ~
                methods:              []
                roles:                []
            role_hierarchy:
                ROLE_ADMIN:      [ROLE_ORGANIZER, ROLE_USER]
                ROLE_SUPERADMIN: [ROLE_ADMIN]

.. _reference-security-firewall-form-login:

フォームログイン設定
--------------------

``form_login`` 認証リスナーをファイアウォールの下で利用する場合において
``form_login`` の構成にはさまざまな用法があります。

さらに詳しくは、 :doc:`/cookbook/security/form_login` も見てみてください。

ログインフォームとプロセス
~~~~~~~~~~~~~~~~~~~~~~~~~~

*   ``login_path`` (type: ``string``, default: ``/login``) は、認証されて
    いないユーザが保護されたリソースへアクセスを試みたときに、リダイレ
    クトする（ ``use_forward`` に ``true`` を設定していない場合）ルート
    またはパスです。

    このパスは認証されていないユーザがアクセス **できなければならず**
    、そうでないとリダイレクトループを生じさせてしまいます。詳しくは、
    ":ref:`Avoid Common Pitfalls <book-security-common-pitfalls>`"を参
    照してください。

*   ``check_path`` (type: ``string``, default: ``/login_check``) は、ログ
    インフォームの送信先であるべきルートまたはパスです。ファイアウォー
    ルはこのURLへのリクエストを捉え（初期値で ``POST`` リクエストのみ）、
    送信されたログイン情報を処理します。

    このURLはメインのファイアウォールの中においてください（つまり
    ``check_path`` のための個別のファイアウォールをつくらないでくださ
    い）。

*   ``use_forward`` (type: ``Boolean``, default: ``false``)
    リダイレクトする代わりに、ユーザをログインフォームへフォワードした
    い場合は、このオプションに ``true`` を設定してください。

*   ``username_parameter`` (type: ``string``, default: ``_username``) は
    ログインフォームでユーザ名を入力するフィールドの名前です。
    ``check_path`` へフォームが送信されると、セキュリティシステムは
    POSTパラメータからこの名前を探します。

*   ``password_parameter`` (type: ``string``, default: ``_password``) は
    ログインフォームでパスワードを入力するフィールドの名前です。
    ``check_path`` へフォームが送信されると、セキュリティシステムは
    POSTパラメータからこの名前を探します。

*   ``post_only`` (type: ``Boolean``, default: ``true``)
    初期値では、 ``check_path`` のURLへのログインフォーム送信はPOSTリク
    エストでなければなりません。このオプションに ``false`` を設定すると、
    ``check_path`` のURLへGETリクエストを送信できるようになります。

ログイン後のリダイレクト
~~~~~~~~~~~~~~~~~~~~~~~~

* ``always_use_default_target_path`` (type: ``Boolean``, default: ``false``)
* ``default_target_path`` (type: ``string``, default: ``/``)
* ``target_path_parameter`` (type: ``string``, default: ``_target_path``)
* ``use_referer`` (type: ``Boolean``, default: ``false``)

.. _reference-security-pbkdf2:

PBKDF2 エンコーダの利用：セキュリティとスピード
-----------------------------------------------

.. versionadded:: 2.2
    PBKDF2 パスワードエンコーダは Symfony 2.2 で追加されました。

`PBKDF2`_ エンコーダは米国国立標準技術研究所（NIST）が推奨する高レベル
な暗号によるセキュリティを提供します。

このページのYAMLブロックで ``pbkdf2`` エンコーダの例を見ることができます。

ただしPBKDF2を利用する場合、大きい数の繰り返しによる計算は処理速度を遅
らせることになります。そのため、PMKDF2は慎重に、注意して利用するべきで
す。

1,000回以上の繰り返しと、ハッシュアルゴリズムのsha512あたりが望ましい設
定です。

.. _reference-security-bcrypt:

BCrypt パスワードエンコーダの利用
---------------------------------

.. caution::

    このエンコーダをつかうには、PHP 5.5をつかうか、Composerから
    `ircmaxell/password-compat`_ ライブラリをインストールする必要があり
    ます。

.. versionadded:: 2.2
    BCrypt パスワードエンコーダは Symfony 2.2 で追加されました。

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...

            encoders:
                Symfony\Component\Security\Core\User\User:
                    algorithm: bcrypt
                    cost:      15

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <!-- ... -->
            <encoder
                class="Symfony\Component\Security\Core\User\User"
                algorithm="bcrypt"
                cost="15"
            />
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            // ...
            'encoders' => array(
                'Symfony\Component\Security\Core\User\User' => array(
                    'algorithm' => 'bcrypt',
                    'cost'      => 15,
                ),
            ),
        ));

``cost`` はエンコードされるパスワードの長さを決定するもので、 ``4-31``
の範囲で設定することができます。``cost`` を増やす度に、パスワードのエン
コードに **２倍** の時間がかかります。

``cost`` に値を与えない場合、初期値の ``13`` がコストに設定されます。

.. note::

    コストはいつでも変更することができますー異なるコストを使用してエン
    コードしたパスワードが既にあったとしても可能です。新しいパスワード
    は新しいコストを使用してエンコードする一方で、既にエンコードされて
    いるものはそれらがエンコードされたときのコストに戻って検証されます。

新しいパスワードのソルトは自動的に生成され、管理する必要はありません。
エンコードされたパスワードはそれをエンコードするために使われたソルトを
含んでいるので、エンコードされたパスワードだけを管理できれば十分です。

.. note::

    すべてのエンコードされたパスワードは ``60`` 文字の長さになるので、
    それらを格納するための領域を十分に割り当ててください。

    .. _reference-security-firewall-context:

ファイアウォールのコンテキスト
------------------------------

ほとんどのアプリケーションはひとつの :ref:`ファイアウォール
<book-security-firewalls>` を必要とします。もし複合ファイアウォールを利
用*していない*場合、ひとつのファイアウォールで認証されたとしても、自動
的に別のファイアウォールで認証されてはいないことに、あなたは気づくでしょ
う。言い換えると、そのシステムは共通の "コンテキスト（文脈）" を共有し
ていません：それぞれのファイアウォールは個別のセキュリティシステムのよ
うに振舞っています。

しかし、それぞれのファイアウォールは任意の ``context`` （デフォルトでファ
イアウォールの名前）、セッション間のセキュリティデータの保存と取得時に
つかわれるキーをもっています。このキーに複合ファイアウォール間で同じ値
を設定していれば、その"コンテキスト"は共有することができます。

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...

            firewalls:
                somename:
                    # ...
                    context: my_context
                othername:
                    # ...
                    context: my_context

    .. code-block:: xml

       <!-- app/config/security.xml -->
       <security:config>
          <firewall name="somename" context="my_context">
            <! ... ->
          </firewall>
          <firewall name="othername" context="my_context">
            <! ... ->
          </firewall>
       </security:config>

    .. code-block:: php

       // app/config/security.php
       $container->loadFromExtension('security', array(
            'firewalls' => array(
                'somename' => array(
                    // ...
                    'context' => 'my_context'
                ),
                'othername' => array(
                    // ...
                    'context' => 'my_context'
                ),
            ),
       ));

HTTPダイジェスト認証
--------------------

HTTPダイジェスト認証を使用するには、レルムとキーを与える必要があります：

.. configuration-block::

   .. code-block:: yaml

      # app/config/security.yml
      security:
         firewalls:
            somename:
              http_digest:
               key: "a_random_string"
               realm: "secure-api"

   .. code-block:: xml

      <!-- app/config/security.xml -->
      <security:config>
         <firewall name="somename">
            <http-digest key="a_random_string" realm="secure-api" />
         </firewall>
      </security:config>

   .. code-block:: php

      // app/config/security.php
      $container->loadFromExtension('security', array(
           'firewalls' => array(
               'somename' => array(
                   'http_digest' => array(
                       'key'   => 'a_random_string',
                       'realm' => 'secure-api',
                   ),
               ),
           ),
      ));

.. _`PBKDF2`: http://en.wikipedia.org/wiki/PBKDF2
.. _`ircmaxell/password-compat`: https://packagist.org/packages/ircmaxell/password-compat

.. 2013/12/07 momoto f4c953673dd6f73a043449c8f8cd6b45846b8f20
