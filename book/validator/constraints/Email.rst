Email
=====

.. Validates that a value is a valid email address.

値が有効なEメールアドレスであることのバリデーションを実行します。

.. code-block:: yaml

    properties:
        email:
            - Email: ~

オプション
----------

.. * ``checkMX``: Whether MX records should be checked for the domain. Default: ``false``
   * ``message``: The error message if the validation fails

* ``checkMX``: ドメインに対してMXレコードをチェックすべきかどうか。デフォルト: ``false``
* ``message``: バリデーションが失敗した時のエラーメッセージ。
