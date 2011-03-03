Regex
=====

.. Validates that a value matches a regular expression.

値が正規表現にマッチするかのバリデーションを実行します。

.. code-block:: yaml

    properties:
        title:
            - Regex: /\w+/

オプション
----------

.. * ``pattern`` (**default**, required): The regular expression pattern
   * ``match``: Whether the pattern must be matched or must not be matched.
     Default: ``true``
   * ``message``: The error message if validation fails

* ``pattern`` (**デフォルト**, 必須): 正規表現のパターン。
* ``match``: パターンがマッチしなければならないかどうか。デフォルト: ``true``
* ``message``: バリデーションが失敗した時のエラーメッセージ。
