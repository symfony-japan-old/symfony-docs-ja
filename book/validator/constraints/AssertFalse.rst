AssertFalse
===========

.. Validates that a value is ``false``.

値が ``false`` であることのバリデーションを実行します。

.. code-block:: yaml

    properties:
        deleted:
            - AssertFalse: ~

オプション
-------

.. * ``message``: The error message if validation fails

* ``message``: バリデーションが失敗した時のエラーメッセージ。

.. See :doc:`AssertTrue <AssertTrue>`.

:doc:`AssertTrue <AssertTrue>` を参照してください。
