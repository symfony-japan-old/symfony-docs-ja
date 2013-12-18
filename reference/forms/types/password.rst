.. index::
   single: Forms; Fields; password

.. note::

    * 対象バージョン：2.4 (2.0以降)
    * 翻訳更新日：2013/12/18

password フィールドタイプ
=========================

``password`` フィールドはパスワードを入力するテキストボックスを表示します。

+-------------+------------------------------------------------------------------------+
| 対応するタグ| ``input`` ``password`` フィールド                                      |
+-------------+------------------------------------------------------------------------+
| オプション  | - `always_empty`_                                                      |
+-------------+------------------------------------------------------------------------+
| 継承された  | - `max_length`_                                                        |
| オプション  | - `required`_                                                          |
|             | - `label`_                                                             |
|             | - `trim`_                                                              |
|             | - `read_only`_                                                         |
|             | - `disabled`_                                                          |
|             | - `error_bubbling`_                                                    |
|             | - `error_mapping`_                                                     |
|             | - `mapped`_                                                            |
+-------------+------------------------------------------------------------------------+
| 親タイプ    | :doc:`text </reference/forms/types/text>`                              |
+-------------+------------------------------------------------------------------------+
| クラス      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\PasswordType` |
+-------------+------------------------------------------------------------------------+

フィールドオプション
--------------------

always_empty
~~~~~~~~~~~~

**データ型**: ``Boolean`` **デフォルト**: ``true``

``true`` にセットした場合、対応するフィールドが値を持つ場合でも *常に* 空白となります。 ``false`` にセットした場合、パスワードフィールドの ``value`` 属性に真の値が設定され表示されます。

平たく言えば、何らかの理由で既に入力されたパスワードと *あわせて* フィールドを表示したい場合は ``false`` をセットしてください。

継承されたオプション
--------------------

以下のオプションは :doc:`form </reference/forms/types/form>` タイプを継承しています:

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. 2013/12/18 yositani2002 4848f40b7de1463e40911bc2871d8990757d0097
