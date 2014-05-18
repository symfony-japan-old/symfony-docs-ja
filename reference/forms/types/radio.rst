.. index::
   single: Forms; Fields; radio

.. note::

   * 対象バージョン：2.3+
   * 翻訳更新日：2014/05/18

radio フィールドタイプ
======================

シングルラジオボタンを作成します。ラジオボタンを選択した場合は、フィールドは指定された値に設定されます。
ラジオボタンはチェックをはずすことはできません - 値は、同じ名前で別のラジオボタンがチェックされるときに変更されます。

``radio`` 型は、通常は直接使用することはありません。
一般的に、 :doc:`choice </reference/forms/types/choice>` のようなほかのタイプの内部で使われます。
``Boolean`` フィールドを使用したい場合は :doc:`checkbox </reference/forms/types/checkbox>` を使用してください。

+------------------+---------------------------------------------------------------------+
| 対応するデータ型 | ``input`` ``radio`` フィールド                                      |
+------------------+---------------------------------------------------------------------+
| 継承された       | - `value`_                                                          |
| オプション       | - `data`_                                                           |
|                  | - `empty_data`_                                                     |
|                  | - `required`_                                                       |
|                  | - `label`_                                                          |
|                  | - `label_attr`_                                                     |
|                  | - `read_only`_                                                      |
|                  | - `disabled`_                                                       |
|                  | - `error_bubbling`_                                                 |
|                  | - `error_mapping`_                                                  |
|                  | - `mapped`_                                                         |
+------------------+---------------------------------------------------------------------+
| 親タイプ         | :doc:`checkbox </reference/forms/types/checkbox>`                   |
+------------------+---------------------------------------------------------------------+
| クラス           | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\RadioType` |
+------------------+---------------------------------------------------------------------+

継承されたオプション
--------------------

以下のオプションは :doc:`checkbox </reference/forms/types/checkbox>` タイプを継承しています:

.. include:: /reference/forms/types/options/value.rst.inc

以下のオプションは :doc:`form </reference/forms/types/form>` タイプを継承しています:

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/empty_data.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

フォーム変数
------------

.. include:: /reference/forms/types/variables/check_or_radio_table.rst.inc

.. 2014/05/18 yositani2002 490633b97f8c2931bc43a76dd060cfcc50da432d
