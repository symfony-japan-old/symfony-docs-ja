.. index::
   single: Tests; HTTP Authentication

ファンクショナルテストで HTTP 認証をシミュレートする方法
========================================================

アプリケーションに HTTP 認証が必要であれば、 ユーザ名とパスワードをサーバの値として ``createClinet()`` に指定してください
::

    $client = static::createClient(array(), array(
        'PHP_AUTH_USER' => 'username',
        'PHP_AUTH_PW'   => 'pa$$word',
    ));

また、各リクエストごとにこの内容をオーバーライドすることもできます。
::

    $client->request('DELETE', '/post/12', array(), array(
        'PHP_AUTH_USER' => 'username',
        'PHP_AUTH_PW'   => 'pa$$word',
    ));

.. 2011/11/02 ganchiku f869a3e48fdd8baafb2aa1859406d5893003bd82

