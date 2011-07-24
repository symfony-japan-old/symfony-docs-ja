.. 2011/07/23 yanchi 36a165e88363fd6e5b5eb0ae712303dd362545be

Email
=====

値が有効なメールアドレスであることを検証します。

.. code-block:: yaml

    properties:
        email:
            - Email: ~

オプション
----------

* ``checkMX``: ドメインに対してMXレコードをチェックするべきかどうか。 デフォルト: ``false``
* ``message``: 検証に失敗した場合のエラーメッセージ
