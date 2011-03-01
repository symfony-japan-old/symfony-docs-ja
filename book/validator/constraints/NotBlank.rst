NotBlank
========

.. Validates that a value is not empty (as determined by the `empty
   <http://php.net/empty>`_ construct).

値が空でないことのバリデーションを実行します。 ( `empty <http://php.net/empty>`_ 定数によって判断されます)

.. code-block:: yaml

    properties:
        firstName:
            - NotBlank: ~

オプション
-------

.. * ``message``: The error message if validation fails

* ``message``: バリデーションが失敗した時のエラーメッセージ。
